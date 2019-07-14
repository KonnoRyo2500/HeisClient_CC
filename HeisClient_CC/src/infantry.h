// heis 兵士クラス
// Author: Ryo Konno
#pragma once

#include "const_val.h"
#include <string>
#include <vector>

class CInfantry
{
	// 構造体，列挙体など
	public:
		// 自分の周囲のマスにいる兵士の情報
		struct NeighborInfantryData {
			Direction direction;
			CInfantry* infantry;
		};

	private:
		// 初期値
		enum InitialValue {
			InitialValue_HP = 2,				// HP
			InitialValue_ActionRemain = 2,		// 残り行動回数
		};

	// メンバ関数
	public:
		// コンストラクタ
		explicit CInfantry(const std::string& team_name, const std::string& infantry_ID, const uint16_t init_pos_x, const uint16_t init_pos_y);

		// デストラクタ
		~CInfantry();

		// ステータスの取得
		std::string get_team_name() const;
		std::string get_id() const;
		uint16_t get_x_position() const;
		uint16_t get_y_position() const;
		uint8_t get_action_remain() const;
		int8_t get_hp() const;

		// 行動
		void attack(const Direction direction);
		void move(const Direction direction);
		std::vector<NeighborInfantryData> look_around() const;

	private:
		void attacked();

		bool is_self(const CInfantry* infantry) const;

		uint16_t get_neighbor_x_pos(const Direction direction) const;
		uint16_t get_neighbor_y_pos(const Direction direction) const;

	// メンバ変数
	private:
		std::string m_team_name;
		std::string m_id;
		uint16_t m_pos_x;
		uint16_t m_pos_y;
		uint8_t m_action_remain;
		// HPについては，値が負になることも考慮して符号付きの値にする(それ以外の値は，負になることはないので符号なしの値にする)
		int8_t m_hp;
};