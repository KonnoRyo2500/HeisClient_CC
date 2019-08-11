﻿// heis 司令官クラス
// Author: Ryo Konno
#pragma once

#include "infantry.h"
#include <vector>

#define INFANTRY_STATUS_ERROR (-1)				// 兵士のステータス取得エラー(指定したIDの兵士がチームの全兵士の中にいないなど)

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
		void attack(const std::string& id, const Direction direction) const;
		void move(const std::string& id, const int16_t delta_x, const int16_t delta_y) const;

		// 兵士の探索
		std::vector<std::string> get_all_actable_infantry_ids() const;
		std::vector<std::string> get_all_movable_infantry_ids() const;
		std::vector<std::string> get_all_attackable_infantry_ids() const;

		// 自チームの全兵士のIDを表示する(デバッグ用)
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
		// 自チーム名
		std::string m_team_name;

		// 自チームの全兵士
		std::vector<CInfantry*> m_infantries;		
};