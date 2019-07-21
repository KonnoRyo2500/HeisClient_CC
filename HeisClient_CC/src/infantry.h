// heis 兵士クラス
// Author: Ryo Konno
#pragma once

#include "const_val.h"
#include "JSON_data_packet.h"
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
		void move(const int16_t delta_x, const int16_t delta_y);
		std::vector<NeighborInfantryData> look_around() const;

		// 「行動」パケットの"contents"配列の1要素分のデータを作成
		ContentsArrayElem create_contents_array_elem() const;

	private:
		// 攻撃された際の処理
		void attacked();

		// 指定されたマスに移動できるかどうかを判定するための関数群
		bool exists_path_for_move(const uint16_t dst_x, const uint16_t dst_y) const;
		std::vector<Direction> decide_move_direction(const uint16_t current_x, const uint16_t current_y, const uint16_t dst_x, const uint16_t dst_y) const;

		// L1距離を計算(移動の際の歩数計算用)
		uint16_t calc_L1_distance(const uint16_t x1, const uint16_t y1, const uint16_t x2, const uint16_t y2) const;

		// 与えられた兵士が自分自身かどうかを判定
		bool is_self(const CInfantry* infantry) const;

		// 隣接したマスの座標を取得
		uint16_t get_neighbor_x_pos(const Direction direction) const;
		uint16_t get_neighbor_x_pos(const Direction direction, const uint16_t ref_x) const;
		uint16_t get_neighbor_y_pos(const Direction direction) const;
		uint16_t get_neighbor_y_pos(const Direction direction, const uint16_t ref_y) const;

	// メンバ変数
	private:
		// 所属チーム名
		std::string m_team_name;

		// ID
		std::string m_id;

		// 位置
		uint16_t m_pos_x;
		uint16_t m_pos_y;

		// 攻撃先の座標(まだ攻撃を行っていない場合には，uint16_t型の最大値が入る)
		uint16_t m_attack_x;
		uint16_t m_attack_y;

		// 残り行動回数
		uint8_t m_action_remain;

		// HP
		// HPについては，値が負になることも考慮して符号付きの値にする(それ以外の値は，負になることはないので符号なしの値にする)
		int8_t m_hp;
};