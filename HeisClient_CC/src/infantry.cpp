// heis 兵士クラス
// Author: Ryo Konno
#include "infantry.h"
#include "field.h"
#include <stdexcept>

/* public関数 */

/*
	コンストラクタ
	引数1: const std::string& team_name 所属チーム名
	引数2: const std::string& infantry_ID ID
	引数3: const uint16_t init_pos_x 初期位置のx座標
	引数4: const uint16_t init_pos_y 初期位置のy座標
*/
CInfantry::CInfantry(const std::string& team_name, const std::string& infantry_ID, const uint16_t init_pos_x, const uint16_t init_pos_y)
	: m_id(infantry_ID)
	, m_hp(InitialValue_HP)
	, m_team_name(team_name)
	, m_pos_x(init_pos_x)
	, m_pos_y(init_pos_y)
	, m_attack_x(UINT16_MAX)
	, m_attack_y(UINT16_MAX)
	, m_action_remain(InitialValue_ActionRemain)
{
	// 呼ばれるタイミングによってはフィールドが未作成の場合もあるので，フィールドが作成済みであることを確認してから配置するようにする
	CField* field = CField::get_instance();
	if (field != NULL && (field->get_width() != 0 && field->get_height() != 0)) {
		field->set_infantry(init_pos_x, init_pos_y, this);
	}
}

/*
	デストラクタ
	引数なし
*/
CInfantry::~CInfantry()
{
	// 呼ばれるタイミングによってはフィールドが未作成の場合もあるので，フィールドが作成済みであることを確認してから削除するようにする
	CField* field = CField::get_instance();
	if (field != NULL && (field->get_width() != 0 && field->get_height() != 0)) {
		field->remove_infantry(m_pos_x, m_pos_y);
	}
}

/*
	兵士のチーム名を取得する関数
	引数なし
	返り値: std::string 兵士のチーム名
*/
std::string CInfantry::get_team_name() const
{
	return m_team_name;
}

/*
	兵士のIDを取得する関数
	引数なし
	返り値: std::string 兵士のID
*/
std::string CInfantry::get_id() const
{
	return m_id;
}

/*
	兵士のx座標を取得する関数
	引数なし
	返り値: uint16_t 兵士のx座標
*/
uint16_t CInfantry::get_x_position() const
{
	return m_pos_x;
}

/*
	兵士のy座標を取得する関数
	引数なし
	返り値: uint16_t 兵士のy座標
*/
uint16_t CInfantry::get_y_position() const
{
	return m_pos_y;
}

/*
	兵士の残り行動回数を取得する関数
	引数なし
	返り値: uint8_t 兵士の残り行動回数
*/
uint8_t CInfantry::get_action_remain() const
{
	return m_action_remain;
}

/*
	兵士のHPを取得する関数
	引数なし
	返り値: int8_t 兵士のHP
*/
int8_t CInfantry::get_hp() const
{
	return m_hp;
}


/*
	指定した方向に攻撃を行う関数
	引数1: const Direction direction 攻撃方向
	返り値なし
*/
void CInfantry::attack(const Direction direction)
{
	if (m_action_remain == 0) {
		fprintf(stderr, "行動回数の上限に達しています\n");
		return;
	}

	CField* field = CField::get_instance();

	try {
		CInfantry* dst_infantry = field->get_infantry(get_neighbor_x_pos(direction), get_neighbor_y_pos(direction));
		if (dst_infantry != NULL) {
			if (dst_infantry->get_team_name() == m_team_name) {
				fprintf(stderr, "味方の兵士を攻撃しようとしています\n");
				return;
			}

			dst_infantry->attacked();
			m_action_remain--;
		}
		else {
			fprintf(stderr, "攻撃しようとした方向に兵士がいません\n");
		}
	}
	catch (std::exception & e) {
		fprintf(stderr, "攻撃に失敗しました\n理由: %s\n", e.what());
	}
}

/*
	指定した方向に兵士を移動する関数
	引数1: const Direction direction 移動方向
	返り値なし
*/
void CInfantry::move(const Direction direction)
{
	if (m_action_remain == 0) {
		fprintf(stderr, "行動回数の上限に達しています\n");
		return;
	}

	CField* field = CField::get_instance();

	try {
		CInfantry* dst_infantry = field->get_infantry(get_neighbor_x_pos(direction), get_neighbor_y_pos(direction));
		if (dst_infantry == NULL) {
			field->remove_infantry(m_pos_x, m_pos_y);
			field->set_infantry(get_neighbor_x_pos(direction), get_neighbor_y_pos(direction), this);

			m_pos_x = get_neighbor_x_pos(direction);
			m_pos_y = get_neighbor_y_pos(direction);

			m_action_remain--;
		}
		else {
			fprintf(stderr, "移動先に兵士がいます\n");
		}
	}
	catch (std::exception & e) {
		fprintf(stderr, "移動に失敗しました\n理由: %s\n", e.what());
	}
}

/*
	自分の周囲4マスを探索する関数
	引数なし
	返り値: std::vector<CInfantry::NeighborInfantryData> 周囲のマスにいる兵士の情報(最大4マス)
*/
std::vector<CInfantry::NeighborInfantryData> CInfantry::look_around() const
{
	std::vector<NeighborInfantryData> neighbor_infantries;
	CField* field = CField::get_instance();

	for (auto direction : {Direction_Up, Direction_Down, Direction_Left, Direction_Right}) {
		CInfantry* infantry = field->get_infantry(get_neighbor_x_pos(direction), get_neighbor_y_pos(direction));

		// ある方向で，自分のいるマスを参照していたら，その方向にはマスが存在していない
		if (!is_self(infantry)) {
			NeighborInfantryData infantry_data = {direction, infantry};

			neighbor_infantries.push_back(infantry_data);
		}
	}

	return neighbor_infantries;
}

/* private関数 */

/*
	敵の兵士からの攻撃を反映させる関数
	引数なし
	返り値なし
*/
void CInfantry::attacked()
{
	m_hp--;
	// 自身の体力が尽きたら，死ぬ(自身をフィールドから消去する)
	// TODO: 今のままだとメモリリークが発生するので，修正する
	if (m_hp <= 0) {
		CField::get_instance()->remove_infantry(m_pos_x, m_pos_y);
	}
}

/*
	与えられた兵士が自分自身なのかを判定する関数
	引数1: const CInfantry* infantry 兵士
	返り値: bool 与えられた兵士が自分自身か
*/
bool CInfantry::is_self(const CInfantry* infantry) const
{
	if (infantry == NULL) {
		return false;
	}
	else if(infantry->get_id() == m_id){
		return true;
	}

	return false;
}

/*
	フィールドのマスのうち，指定した方向に隣接したマスのx座標を取得する関数
	引数1: const Direction direction 方向
	返り値: uint16_t x座標
*/
uint16_t CInfantry::get_neighbor_x_pos(const Direction direction) const
{
	uint16_t dst_x_pos;

	switch (direction) {
		case Direction_Left:
			dst_x_pos = m_pos_x - 1;
			break;
		case Direction_Right:
			dst_x_pos = m_pos_x + 1;
			break;
		default:
			dst_x_pos = m_pos_x;
			break;
	}

	if (dst_x_pos < 0) {
		dst_x_pos = 0;
	}
	else if (FieldParam_Width <= dst_x_pos) {
		dst_x_pos = FieldParam_Width - 1;
	}

	return dst_x_pos;
}

/*
	フィールドのマスのうち，指定した方向に隣接したマスのy座標を取得する関数
	引数1: const Direction direction 方向
	返り値: uint16_t y座標
*/
uint16_t CInfantry::get_neighbor_y_pos(const Direction direction) const
{
	uint16_t dst_y_pos;

	switch (direction) {
	case Direction_Up:
		dst_y_pos = m_pos_y - 1;
		break;
	case Direction_Down:
		dst_y_pos = m_pos_y + 1;
		break;
	default:
		dst_y_pos = m_pos_y;
		break;
	}

	if (dst_y_pos < 0) {
		dst_y_pos = 0;
	}
	else if (FieldParam_Height <= dst_y_pos) {
		dst_y_pos = FieldParam_Height - 1;
	}

	return dst_y_pos;
}