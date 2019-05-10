﻿// heis 兵士クラス
// Author: Ryo Konno
#include "infantry.h"

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
{
	// 処理なし
}

/*
	兵士のHPを取得する関数
	引数なし
	返り値: uint8_t 兵士のHP
*/
uint8_t CInfantry::get_hp()
{
	return m_hp;
}

/*
	兵士のx座標を取得する関数
	引数なし
	返り値: uint16_t 兵士のx座標
*/
uint16_t CInfantry::get_x_position()
{
	return m_pos_x;
}

/*
	兵士のy座標を取得する関数
	引数なし
	返り値: Position 兵士のy座標
*/
uint16_t CInfantry::get_y_position()
{
	return m_pos_y;
}

/*
	指定した方向に攻撃を行う関数
	引数1: Direction direction 攻撃方向
	返り値なし
*/
void CInfantry::attack(Direction direction)
{
	
}

/*
	指定した方向に兵士を移動する関数
	引数1: Direction direction 移動方向
	返り値なし
*/
void CInfantry::move(Direction direction)
{

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