﻿// heis 兵士クラス
// Author: Ryo Konno
#pragma once

#include "const_val.h"
#include <string>

class CInfantry
{
	// 構造体，列挙体など
	private:
		// 初期値
		enum InitialValue {
			InitialValue_HP = 2,				// HP
			InitialValue_ActionRemain = 2,		// 残り行動回数
		};

	// メンバ関数
	public:
		// コンストラクタ
		explicit CInfantry(const std::string& team_name, const uint16_t infantry_ID, const uint16_t init_pos_x, const uint16_t init_pos_y);

		// ステータスの取得
		std::string get_team_name() const;
		uint16_t get_id() const;
		uint16_t get_x_position() const;
		uint16_t get_y_position() const;
		uint8_t get_action_remain() const;
		uint8_t get_hp() const;

		// 行動
		void attack(const Direction direction);
		void move(const Direction direction);

		// 内部処理用(ユーザAIはこの処理を呼ばないこと)
		void update_status();

	private:
		void attacked();
		void reset_action_remain();

		bool is_self(const CInfantry* infantry) const;

		uint16_t calc_neighbor_x_pos(const Direction direction) const;
		uint16_t calc_neighbor_y_pos(const Direction direction) const;

	// メンバ変数
	private:
		std::string m_team_name;
		uint16_t m_id;
		uint16_t m_pos_x;
		uint16_t m_pos_y;
		uint8_t m_action_remain;
		uint8_t m_hp;
};