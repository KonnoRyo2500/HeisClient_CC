// heis 兵士クラス
// Author: Ryo Konno
#include "infantry.h"
#include "field.h"

/* public関数 */

/*
	コンストラクタ
	引数2: int infantry_ID 兵士ID
*/
CInfantry::CInfantry(const std::string& team_name, const uint16_t infantry_ID, const uint16_t init_pos_x, uint16_t init_pos_y)
	: m_id(infantry_ID)
	, m_hp(InitialValue_HP)
	, m_team_name(team_name)
	, m_pos_x(init_pos_x)
	, m_pos_y(init_pos_y)
	, m_action_remain(InitialValue_ActionNum)
{
	CField::get_instance()->set_infantry(init_pos_x, init_pos_y, this);
}

/*
	兵士のHPを取得する関数
	引数なし
	返り値: uint8_t 兵士のHP
*/
uint8_t CInfantry::get_hp() const
{
	return m_hp;
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
	返り値: Position 兵士のy座標
*/
uint16_t CInfantry::get_y_position() const
{
	return m_pos_y;
}

/*
	指定した方向に攻撃を行う関数
	引数1: const Direction direction 攻撃方向
	返り値なし
*/
void CInfantry::attack(const Direction direction)
{
	if (m_action_remain == 0) {
		printf("行動回数の上限に達しています\n");
		return;
	}

	CField* field = CField::get_instance();
	CInfantry* dst_infantry = field->get_infantry(calc_neighbor_x_pos(direction), calc_neighbor_y_pos(direction));

	if (dst_infantry != NULL) {
		dst_infantry->attacked();
		m_action_remain--;
	}
	else {
		printf("攻撃しようとした方向に兵士がいません\n");
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
		printf("行動回数の上限に達しています\n");
		return;
	}

	CField* field = CField::get_instance();
	CInfantry* dst_infantry = field->get_infantry(calc_neighbor_x_pos(direction), calc_neighbor_y_pos(direction));

	if (dst_infantry == NULL) {
		field->set_infantry(m_pos_x, m_pos_y, NULL);
		field->set_infantry(calc_neighbor_x_pos(direction), calc_neighbor_y_pos(direction), this);

		m_pos_x = calc_neighbor_x_pos(direction);
		m_pos_y = calc_neighbor_y_pos(direction);

		m_action_remain--;
	}
	else {
		printf("移動先に兵士がいます\n");
	}
}

/* private関数 */

/*
	敵の兵士からの攻撃を反映させる関数
	引数なし
	返り値なし
	注意: この関数は，内部的な処理を行うためのものなので，ユーザAIはこの関数を呼んではいけない
*/
void CInfantry::attacked()
{
	m_hp--;
}

/*
	フィールドのマスのうち，指定した方向に隣接したマスのx座標を取得する関数
	引数1: const Direction direction 方向
	返り値: uint16_t x座標
*/
uint16_t CInfantry::calc_neighbor_x_pos(const Direction direction) const
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

	return dst_x_pos;
}

/*
	フィールドのマスのうち，指定した方向に隣接したマスのy座標を取得する関数
	引数1: const Direction direction 方向
	返り値: uint16_t y座標
*/
uint16_t CInfantry::calc_neighbor_y_pos(const Direction direction) const
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

	return dst_y_pos;
}