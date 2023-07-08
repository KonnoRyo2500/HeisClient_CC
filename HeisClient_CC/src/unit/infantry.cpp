/**
*	@file		infantry.cpp
*	@brief		heis 兵士クラス
*	@author		Ryo Konno
*	@details	heisのユニットの1つである兵士ユニットを定義する．
*/
#include "infantry.h"
#include "board.h"
#include <stdexcept>
#include <queue>

/* public関数 */

/**
*	@brief コンストラクタ
*	@param[in] team_name 所属チーム名
*	@param[in] id ID
*	@param[in] hp 初期HP(「盤面」JSONから再配置する際に使用する)
*/
CInfantry::CInfantry(const InfantryStatus& init_status)
	: m_status(init_status)
{
	// Do Nothing
}

/**
*	@brief デストラクタ
*/
CInfantry::~CInfantry()
{
	// Do Nothing
}

/**
*	@brief 現在のステータスを取得する
*	@return InfantryStatus 現在のステータス
*/
InfantryStatus CInfantry::get_status() const
{
	return m_status;
}

/**
*	@brief HPを1減らす
*	@remark すでにHPが0の場合は何もしない
*/
void CInfantry::reduce_hp()
{
	if (m_status.hp >= 1) {
		m_status.hp--;
	}
}

/**
*	@brief 行動回数を指定された回数減らす
*	@param[in] n_reduce 行動回数の減少分
*	@remark 行動回数は0以下にはならない
*/
void CInfantry::reduce_remaining_action(uint8_t n_reduce)
{
	if (m_status.action_remain >= n_reduce) {
		m_status.action_remain -= n_reduce;
	}
	else {
		m_status.action_remain = 0;
	}
}

/**
*	@brief 自分が死んでいるかを判定する
*	@return bool 自分が死んでいるか
*/
bool CInfantry::is_dead() const
{
	return m_status.hp == 0;
}