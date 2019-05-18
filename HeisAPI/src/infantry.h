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
		explicit CInfantry(const std::string& team_name, const std::string& infantry_ID, const int16_t init_pos_x, const int16_t init_pos_y);

		// デストラクタ
		~CInfantry();

		// ステータスの取得
		std::string get_team_name() const;
		std::string get_id() const;
		int16_t get_x_position() const;
		int16_t get_y_position() const;
		int8_t get_action_remain() const;
		int8_t get_hp() const;

		// 行動
		void attack(const Direction direction);
		void move(const Direction direction);
		std::vector<NeighborInfantryData> look_around() const;

		// 内部処理用(ユーザAIはこの処理を呼ばないこと)
		void update_status();

	private:
		void attacked();
		void reset_action_remain();

		bool is_self(const CInfantry* infantry) const;

		int16_t get_neighbor_x_pos(const Direction direction) const;
		int16_t get_neighbor_y_pos(const Direction direction) const;

	// メンバ変数
	private:
		std::string m_team_name;
		std::string m_id;
		// エラー値(負の値)も表現したいので，符号付きの値にする
		int16_t m_pos_x;
		int16_t m_pos_y;
		int8_t m_action_remain;
		int8_t m_hp;
};