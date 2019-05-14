﻿// heis 司令官クラス
// Author: Ryo Konno
#pragma once

#include "infantry.h"
#include <vector>

class CCommander
{
	// 構造体，列挙体など
	private:

	// メンバ関数
	public:
		// コンストラクタ
		explicit CCommander(const std::string& team_name);

		// ステータスの取得
		uint16_t get_x_position(const std::string& id) const;
		uint16_t get_y_position(const std::string& id) const;
		uint8_t get_action_remain(const std::string& id) const;
		uint8_t get_hp(const std::string& id) const;

		// 行動(指定したIDを持つ兵士への命令)
		void attack(const std::string& id, const Direction direction) const;
		void move(const std::string& id, const Direction direction) const;
		void double_move(const std::string& id, const Direction first_move_direction, const Direction second_move_direction) const;

		// 行動可能な兵士の取得
		std::vector<std::string&> get_all_movable_infantry_ids();

		// 内部処理用(ユーザAIはこの処理を呼ばないこと)
		void update_all_infantries();

	private:
		CInfantry* find_infantry_by_id(const std::string& id) const;

	// メンバ変数
	private:
		std::string m_team_name;					// チーム名
		std::vector<CInfantry*> m_infantries;		// チームの全兵士
};