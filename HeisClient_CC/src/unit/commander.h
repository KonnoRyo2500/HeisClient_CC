/**
*	@file		commander.h
*	@brief		heis 司令官クラス
*	@author		Ryo Konno
*	@details	チームの兵士を一元管理し，操作や状態取得を行う．
*/
#pragma once

#include "infantry.h"
#include "log.h"
#include <vector>

#define INFANTRY_STATUS_ERROR (-1)				// 兵士のステータス取得エラー(指定したIDの兵士がチームの全兵士の中にいないなど)

extern CLog g_battle_log;

/**
*	@brief	司令官クラス
*/
class CCommander
{
	// 構造体，列挙体など
	public:

	// メンバ関数
	public:
		// コンストラクタ
		explicit CCommander(const std::string& team_name);

		// デストラクタ
		~CCommander();

		// 指定したIDを持つ兵士のステータス取得
		FieldPosition get_position(const std::string& id) const;
		uint8_t get_action_remain(const std::string& id) const;
		int8_t get_hp(const std::string& id) const;

		// 行動(指定したIDを持つ兵士への命令)
		void attack(const std::string& id, const FieldPosition dst_pos) const;
		void move(const std::string& id, const FieldPosition dst_pos) const;
		std::vector<FieldPosition> find_movable_position(const std::string& id) const;
		std::vector<FieldPosition> find_attackable_position(const std::string& id) const;

		// 兵士の探索
		std::vector<std::string> get_all_actable_infantry_ids(const std::string& team_name) const;
		std::vector<std::string> get_all_movable_infantry_ids(const std::string& team_name) const;
		std::vector<std::string> get_all_attackable_infantry_ids(const std::string& team_name) const;

		// フィールドにいる全兵士のIDを表示する(デバッグ用)
		void show_infantry_ids() const;

		// 「行動」パケットを作成
		JSONSendPacket_Action create_action_pkt() const;

		// 兵士の情報を更新
		void update();

	private:
		// 兵士の探索
		CInfantry* search_infantry_by_id(const std::string& id) const;
		bool is_actable(const CInfantry* infantry) const;
		bool is_movable(const CInfantry* infantry) const;
		bool is_attackable(const CInfantry* infantry) const;

	// メンバ変数
	private:
		//! 自チーム名
		std::string m_team_name;

		//! フィールドにいる全兵士
		std::vector<CInfantry*> m_infantries;		
};