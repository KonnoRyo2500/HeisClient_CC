/**
*	@file		field.cpp
*	@brief		heis フィールドクラス
*	@author		Ryo Konno
*	@details	heisの各種ユニットが配置されるフィールドの定義およびそれに対する操作を提供する．
*/
#include "field.h"
#include "common.h"

CField* CField::m_instance = NULL;

const std::string CField::EMPTY_ID = "";

/* 注) 座標は，最も左上のマスを(0, 0)とする */

/* public関数 */

/**
*	@brief デストラクタ
*/
CField::~CField()
{
	delete_all_infantries();
}

/**
*	@brief フィールドのインスタンスを取得する関数
*	@return CField* フィールドのインスタンス(へのポインタ)
*/
CField* CField::get_instance()
{
	return m_instance;
}

/**
*	@brief フィールドを作成する関数
*/
void CField::create_field()
{
	if (m_instance == NULL) {
		m_instance = new CField;
		m_instance->initalize();
	}
}

/**
*	@brief フィールドを削除する関数
*/
void CField::delete_field()
{
	delete m_instance;
	m_instance = NULL;
}

/**
*	@brief 指定された座標にいる兵士を取得する関数
*	@param[in] pos 取得したい兵士のいる座標
*	@return CInfantry* 指定された座標にいる兵士(存在しなければNULL)
*/
CInfantry* CField::get_infantry(const FieldPosition& pos) const
{
	// 指定された座標が不正でないかどうかチェック
	validate_position(pos);

	CInfantry* infantry = find_infantry_by_id(m_grid[pos.x + (m_width * pos.y)]);
	if (infantry != NULL) {
		return infantry;
	}
	return NULL;
}

/**
*	@brief 指定した座標に兵士を配置する関数
*	@param[in] pos 兵士を配置する座標
*	@param[in] infantry 配置する兵士
*	@throws std::runtime_error NULLの兵士を配置しようとしたとき
*/
void CField::set_infantry(const FieldPosition& pos, CInfantry* infantry)
{
	// 指定された座標が不正でないかどうかチェック
	validate_position(pos);
	// 兵士がNULLの場合は，IDを取得できないので不正
	if (infantry == NULL) {
		throw std::runtime_error("配置しようとしている兵士がNULLです．remove_infantry関数を呼ぶようにしてください");
	}

	add_infantry(infantry);
	m_grid[pos.x + (m_width * pos.y)] = infantry->get_id();
}

/**
*	@brief 指定した座標にいる兵士を削除する関数
*	@param[in] pos 削除する兵士のいる座標
*	@remark 指定された座標に兵士がいない場合でもエラーにはしない
*/
void CField::remove_infantry(const FieldPosition& pos)
{
	// 指定された座標が不正でないかどうかチェック
	validate_position(pos);

	/*
		実体はリスト(m_all_infantries)に残り続けるが，update関数で呼ばれるdelete_all_infantries関数で
		リスト中のすべての兵士が削除されるため，この実装でもメモリリークは発生しない
	*/
	m_grid[pos.x + (m_width * pos.y)] = EMPTY_ID;
}

/**
*	@brief フィールドの幅を取得する関数
*	@return uint16_t フィールドの幅
*/
uint16_t CField::get_width()
{
	return m_width;
}

/**
*	@brief フィールドの高さを取得する関数
*	@return uint16_t フィールドの高さ
*/
uint16_t CField::get_height()
{
	return m_height;
}

/**
*	@brief 「盤面」パケットの内容から，フィールドの状態を更新する関数
*	@param[in] field_pkt 「盤面」パケット
*/
void CField::update(const JSONRecvPacket_Field& field_pkt)
{
	// 最初の「盤面」パケットを受け取ったときに，フィールドのマス目が構成される
	if (m_width == 0 && m_height == 0) {
		m_width = field_pkt.width;
		m_height = field_pkt.height;
		create_grid(field_pkt.width, field_pkt.height);
	}
	validate_size(field_pkt.width, field_pkt.height);

	// フィールドの各兵士の状態を，「盤面」パケットに記載された各兵士の状態と合わせる
	delete_all_infantries();
	clear_grid();
	relocate_all_infantries_from_units_array(field_pkt.units);
}

/**
*	@brief フィールドを画面に表示する関数
*/
void CField::show()
{
	std::string show_str = "";

	for (int y = 0; y < m_height; y++) {
		for (int x = 0; x < m_width; x++) {
			CInfantry* infantry = get_infantry(FieldPosition(x, y));

			if (infantry == NULL) {
				show_str += "----";
			}
			else {
				show_str += infantry->get_id().c_str();
			}
			show_str += " ";
		}
		show_str += "\n";
	}
	show_str += "\n";

	// ログの先頭情報により，盤面が崩れて見えるのを防ぐため，先頭に改行を入れておく
	g_field_log->write_log(CLog::LogType_Infomation, true, "\n%s", show_str.c_str());
}

/* private関数 */

/**
*	@brief フィールドを初期化する関数
*/
void CField::initalize()
{
	m_width = 0;
	m_height = 0;
}

/**
*	@brief 指定されたサイズで，フィールドのマス目を作成する関数
*	@param[in] width 作成するフィールドの幅
*	@param[in] height 作成するフィールドの高さ
*	@remark 作成される各マス目は空
*/
void CField::create_grid(const uint16_t width, const uint16_t height)
{
	validate_size(width, height);
	m_grid.resize(width * height);
	std::fill(m_grid.begin(), m_grid.end(), EMPTY_ID);
}

/**
*	@brief フィールドのすべてのマス目を空にする関数
*/
void CField::clear_grid()
{
	std::fill(m_grid.begin(), m_grid.end(), EMPTY_ID);
}

/**
*	@brief 指定されたIDから，兵士リスト中の兵士を取得する関数
*	@param[in] infantry_id 対象の兵士のID
*	@return CInfantry* 取得した兵士(兵士リスト中に，infantry_idをIDとして持つ兵士がいなければNULL)
*/
CInfantry* CField::find_infantry_by_id(const std::string& infantry_id) const
{
	auto infantry_it = m_all_infantries.find(infantry_id);
	return infantry_it != m_all_infantries.end() ? infantry_it->second : NULL;
}

/**
*	@brief 兵士リスト中の兵士をすべて削除する関数
*/
void CField::delete_all_infantries()
{
	// 兵士のリストをクリア
	/*
		mapだとイテレータを使う関係上すべての要素についてdeleteができないので，
		一旦兵士の実体をvectorにコピーしてからdeleteする
	*/
	std::vector<CInfantry*> del_infantries;
	for (auto& it : m_all_infantries) {
		del_infantries.push_back(it.second);
	}
	for (CInfantry* del_infatry : del_infantries) {
		delete del_infatry;
		del_infatry = NULL;
	}
	m_all_infantries.clear();
}

/**
*	@brief 兵士リストに，兵士を追加する関数
*	@param[in] new_infantry 追加する兵士
*/
void CField::add_infantry(CInfantry* new_infantry)
{
	m_all_infantries.insert(std::make_pair(new_infantry->get_id(), new_infantry));
}

/**
*	@brief 「盤面」パケットの"units"配列の情報から，兵士を再配置する関数
*	@param[in] units_array 「盤面」パケットの"units"配列
*/
void CField::relocate_all_infantries_from_units_array(const std::vector<UnitsArrayElem>& units_array)
{
	for (auto& infantry_data : units_array) {
		CInfantry* infantry = new CInfantry(infantry_data.team, infantry_data.unit_id, FieldPosition(infantry_data.locate.x, infantry_data.locate.y));
		set_infantry( FieldPosition(infantry_data.locate.x, infantry_data.locate.y), infantry);
	}
}

/**
*	@brief 座標を検証する関数
*	@param[in] pos 検証対象の座標
*	@throws std::runtime_error 指定した座標が範囲外のとき
*/
void CField::validate_position(const FieldPosition& pos) const
{
	// if文で条件判定すると条件式が長くなるのでこのような書き方をした
	bool is_position_valid = (0 <= pos.x && pos.x < m_width);

	is_position_valid &= (0 <= pos.y && pos.y < m_height);
	if (!is_position_valid) {
		throw std::runtime_error(cc_common::format("指定された座標(%d, %d)は不正です", pos.x, pos.y));
	}
}

/**
*	@brief 「盤面」パケットから得られたフィールドのサイズを検証する関数
*	@param[in] width フィールドの幅
*	@param[in] height フィールドの高さ
*	@throws std::runtime_error フィールドの幅もしくは高さが0以下のとき
*	@throws std::runtime_error フィールドの幅もしくは高さが過去に受け取った「盤面」パケットの値と異なるとき
*/
void CField::validate_size(const uint16_t width, const uint16_t height) const
{
	if (width <= 0 || height <= 0) {
		throw std::runtime_error(cc_common::format("フィールドのサイズが不正です(サイズ: %d * %d)", width, height));
	}
	if (width != m_width || height != m_height) {
		throw std::runtime_error(cc_common::format("フィールドのサイズが，過去に受信した「盤面」パケットから得られたサイズと異なります(今回のサイズ: %d * %d, これまでのサイズ: %d * %d)", width, height, m_width, m_height));
	}
}