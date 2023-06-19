/**
*	@file		infantry.h
*	@brief		heis 兵士クラス
*	@author		Ryo Konno
*	@details	heisのユニットの1つである兵士ユニットを定義する．
*/
#pragma once

#include "const_val.h"
#include "common_structure.h"
#include "JSON_data_packet.h"

#include <string>
#include <vector>

/**
*	@struct InfantryStatus
*	@brief 兵士のステータス
*/
struct InfantryStatus
{
	/**
	*	@brief コンストラクタ
	*	@param[in] team_name チーム名
	*	@param[in] id ID
	*	@param[in] hp 初期HP
	*	@param[in] action_remain 初期残り行動回数
	*/
	explicit InfantryStatus(
		const std::string& team_name,
		const std::string& id,
		uint8_t hp = INFANTRY_INITIAL_HP,
		uint8_t action_remain = INFANTRY_ACTION_LIMIT
	)
		: team_name(team_name)
		, id(id)
		, action_remain(action_remain)
		, hp(hp)
	{
		// Do Nothing
	}

	//! チーム名
	std::string team_name;
	//! ID
	std::string id;
	//! 残り行動回数
	uint8_t action_remain;
	//! HP
	uint8_t hp;
};

/**
*	@brief	兵士クラス
*/
class CInfantry
{
	// メンバ関数
	public:
		// コンストラクタ
		explicit CInfantry(const InfantryStatus& init_status);

		// デストラクタ
		~CInfantry();

		// ステータスを取得する
		InfantryStatus get_status() const;

		// HPを1減らす
		void reduce_hp();
		// 行動回数を指定された回数減らす
		void reduce_remaining_action(const uint8_t n_reduce);

		// 死んでいるかどうかの判定
		bool is_dead() const;

	// メンバ変数
	private:
		//! 現在のステータス
		InfantryStatus m_status;
};