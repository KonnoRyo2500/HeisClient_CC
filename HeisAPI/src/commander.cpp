// heis 司令官クラス
// Author: Ryo Konno

#include "commander.h"

/* public関数 */

/*
	コンストラクタ
	引数1: const std::string& team_name チーム名
	備考: こちらはオンラインモードの処理で呼ぶ
*/
CCommander::CCommander(const std::string& team_name)
	: m_team_name(team_name)
{
	// 処理なし(あれば初期化処理)
}

/*
	コンストラクタ
	引数1: const std::string& team_name チーム名
	引数2: const int init_area_width 初期化領域の幅
	引数3: const int init_area_height 初期化領域の高さ
	引数4: const bool is_bottom_left 初期化領域を左下にするか
	備考: こちらはローカルモードの処理で呼ぶ
*/
CCommander::CCommander(const std::string& team_name, const int init_area_width, const int init_area_height, const bool is_bottom_left)
	: m_team_name(team_name)
{
	// 兵士の初期配置を行う
	positioning_infantries(init_area_width, init_area_height, is_bottom_left);
}

/*
	デストラクタ
	引数なし
*/
CCommander::~CCommander()
{
	clear_infantries();
}

/*
	指定したIDを持つ兵士のx座標を取得する関数
	引数1: (const std::string& id ID
	返り値: int16_t 兵士のx座標(指定したIDの兵士がいなければ-1)
*/
int16_t CCommander::get_x_position(const std::string& id) const
{
	CInfantry* infantry = search_infantry_by_id(id);

	if (infantry != NULL) {
		return infantry->get_x_position();
	}
	else {
		return INFANTRY_STATUS_ERROR;
	}
}

/*
	指定したIDを持つ兵士のy座標を取得する関数
	引数1: (const std::string& id ID
	返り値: int16_t 兵士のy座標(指定したIDの兵士がいなければ-1)
*/
int16_t CCommander::get_y_position(const std::string& id) const
{
	CInfantry* infantry = search_infantry_by_id(id);

	if (infantry != NULL) {
		return infantry->get_y_position();
	}
	else {
		return INFANTRY_STATUS_ERROR;
	}
}

/*
	指定したIDを持つ兵士の残り行動回数を取得する関数
	引数1: (const std::string& id ID
	返り値: int16_t 兵士の残り行動回数(指定したIDの兵士がいなければ-1)
*/
int8_t CCommander::get_action_remain(const std::string& id) const
{
	CInfantry* infantry = search_infantry_by_id(id);

	if (infantry != NULL) {
		return infantry->get_action_remain();
	}
	else {
		return INFANTRY_STATUS_ERROR;
	}
}

/*
	指定したIDを持つ兵士のHPを取得する関数
	引数1: (const std::string& id ID
	返り値: int16_t 兵士のHP(指定したIDの兵士がいなければ-1)
*/
int8_t CCommander::get_hp(const std::string& id) const
{
	CInfantry* infantry = search_infantry_by_id(id);

	if (infantry != NULL) {
		return infantry->get_hp();
	}
	else {
		return INFANTRY_STATUS_ERROR;
	}
}

/*
	指定したIDの兵士に攻撃を行わせる関数
	引数1: const std::string& id ID
	引数2: const Direction direction 攻撃方向
	返り値なし
*/
void CCommander::attack(const std::string& id, const Direction direction) const
{
	CInfantry* infantry = search_infantry_by_id(id);

	if (infantry != NULL) {
		infantry->attack(direction);
	}
}

/*
	指定したIDの兵士を移動する関数
	引数1: const std::string& id ID
	引数2: const Direction direction 移動方向
	返り値なし
*/
void CCommander::move(const std::string& id, const Direction direction) const
{
	CInfantry* infantry = search_infantry_by_id(id);

	if (infantry != NULL) {
		infantry->move(direction);
	}
}

/*
	移動可能なすべての兵士のIDを取得する関数
	引数なし
	返り値: std::vector<std::string&> 移動可能な各兵士のID
	備考: ユーザAIは，この関数を呼んでから具体的な行動(探索など)を行うとよい
*/
std::vector<std::string> CCommander::get_all_movable_infantry_ids() const
{
	std::vector<std::string> movable_infantry_ids;

	for (CInfantry* my_team_infantry : m_infantries) {
		if (is_movable(my_team_infantry)) {
			movable_infantry_ids.push_back(my_team_infantry->get_id());
		}
	}

	return movable_infantry_ids;
}

/*
	自チームの兵士IDをすべて表示する関数
	備考: 動作確認・デバッグ用
*/
void CCommander::show_infantry_ids()
{
	printf("%s: ", m_team_name.c_str());
	for (CInfantry* infantry : m_infantries) {
		printf("%s ", infantry->get_id().c_str());
	}
	printf("\n");
}

/* private関数 */

/*
	兵士を固定の領域に配置する関数
	引数1: const int init_area_width 初期化領域の幅
	引数2: const int init_area_height 初期化領域の高さ
	引数3: const bool is_bottom_left 初期化領域が左下か
	返り値なし
	備考: この関数は，ローカルモードで用いられる
*/
void CCommander::positioning_infantries(const int init_area_width, const int init_area_height, const bool is_bottom_left)
{
	int num_infantry = 0;

	for (int16_t y = 0; y < init_area_height; y++) {
		for (int16_t x = 0; x < init_area_width; x++) {
			if (is_bottom_left) {
				append_infantry(make_id(m_team_name.substr(0, 2), num_infantry), x, y);
			}
			else {
				append_infantry(make_id(m_team_name.substr(0, 2), num_infantry), FieldParam_Width - (init_area_width - x), FieldParam_Height - (init_area_height - y));
			}
			num_infantry++;
		}
	}
}

/*
	自チームのすべての兵士を削除する関数
	引数なし
	返り値なし
*/
void CCommander::clear_infantries()
{
	for (CInfantry* infantry : m_infantries) {
		delete infantry;
		infantry = NULL;
	}

	m_infantries.clear();
}

/*
	兵士を自チームに追加する関数
	引数1: const std::string& id 追加する兵士のID
	引数2: const int16_t pos_x 追加する兵士のx座標
	引数3: const int16_t pos_y 追加する兵士のy座標
	返り値なし
*/
void CCommander::append_infantry(const std::string& id, const int16_t pos_x, const int16_t pos_y)
{
	if (search_infantry_by_id(id) != NULL) {
		return;
	}

	CInfantry* new_infantry = new CInfantry(m_team_name, id, pos_x, pos_y);

	m_infantries.push_back(new_infantry);
}

/*
	兵士のIDを作成する関数
	引数1: const std::string& prefix 接頭辞(IDの先頭につく2文字のアルファベット)
	引数2: const int16_t number IDの数字部分
	返り値: std::string 作成されたID
*/
std::string CCommander::make_id(const std::string& prefix, const int16_t number)
{
	char id[10];

	if (number >= 100) {
		printf("警告: 兵士IDが衝突する恐れがあります\n");
	}
	snprintf(id, sizeof(id) / sizeof(char), "%s%02d", prefix.c_str(), number);

	return std::string(id);
}

/*
	兵士を自チームから削除する関数
	引数1: const std::string& id 削除する兵士のID
	返り値なし
*/
void CCommander::remove_infantry(const std::string& id)
{
	CInfantry* del_infantry = search_infantry_by_id(id);

	if (del_infantry == NULL) {
		return;
	}

	auto infantry_it = std::find(m_infantries.begin(), m_infantries.end(), del_infantry);

	if (infantry_it != m_infantries.end()) {
		m_infantries.erase(infantry_it);
	}
	delete del_infantry;
}

/*
	全兵士のリストから，与えられたIDを持つ兵士を探す関数
	引数1: const std::string& id ID
	返り値: CInfantry* 与えられたIDを持つ兵士(存在しなければNULL)
*/
CInfantry* CCommander::search_infantry_by_id(const std::string& id) const
{
	for (auto infantry : m_infantries) {
		if (infantry->get_id() == id) {
			return infantry;
		}
	}

	return NULL;
}

/*
	与えられた兵士が隣接したマスに移動可能かを調べる関数
	引数1: const CInfantry* infantry 兵士
	返り値: bool 兵士が移動可能か(true: 移動可能なマスがある, false: 隣接したどのマスにも移動できない)
*/
bool CCommander::is_movable(const CInfantry* infantry) const
{
	for (auto look_result : infantry->look_around()) {
		if (look_result.infantry == NULL) {
			return true;
		}
	}

	return false;
}