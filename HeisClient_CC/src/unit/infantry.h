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
*	@brief	兵士クラス
*/
class CInfantry
{
	// 構造体，列挙体など
	public:

	private:
		/**
		*	@enum InitialValue
		*	兵士ユニットに関する初期値
		*/
		enum InitialValue {
			//! HP
			InitialValue_HP = 2,
			//! 残り行動回数
			InitialValue_ActionRemain = 2,
		};

	// メンバ関数
	public:
		// コンストラクタ
		explicit CInfantry(const std::string& team_name, const std::string& infantry_id, const BoardPosition& init_pos);

		// デストラクタ
		~CInfantry();

		// ステータスの取得
		std::string get_team_name() const;
		std::string get_id() const;
		BoardPosition get_position() const;
		uint8_t get_action_remain() const;
		int8_t get_hp() const;

		// 行動
		void attack(const BoardPosition dst_pos);
		void move(const BoardPosition dst_pos);

		// 周囲の探索
		std::vector<BoardPosition> find_movable_position() const;
		std::vector<BoardPosition> find_attackable_position() const;

		// 「行動」パケットの"contents"配列の1要素分のデータを作成
		ContentsArrayElem create_contents_array_elem() const;

	private:
		// 攻撃された際の処理
		void attacked();

		// 指定されたマスに移動できるかどうかを判定するための関数群
		bool exists_path_for_move(const BoardPosition& dst_pos) const;
		std::vector<Direction> decide_move_direction(const BoardPosition& current_pos, const BoardPosition& dst_pos) const;

		// L1距離を計算(移動の際の歩数計算用)
		uint16_t calc_L1_distance(const BoardPosition& src, const BoardPosition dst) const;

		// 自分の周囲のマスを取得
		std::vector<BoardPosition> get_around_position(const uint16_t search_distance) const;

		// 与えられた兵士が自分自身かどうかを判定
		bool is_self(const CInfantry* infantry) const;

		// 隣接したマスの座標を取得
		BoardPosition get_neighbor_pos(const Direction direction, const BoardPosition& origin) const;

	// メンバ変数
	private:
		//! 所属チーム名
		std::string m_team_name;

		//! ID
		std::string m_id;

		//! 位置
		BoardPosition m_pos;

		//! 攻撃先の座標(まだ攻撃を行っていない場合には，uint16_t型の最大値が入る)
		BoardPosition m_attack_pos;

		//! 残り行動回数
		uint8_t m_action_remain;

		// HPについては，値が負になることも考慮して符号付きの値にする(それ以外の値は，負になることはないので符号なしの値にする)
		//! HP
		int8_t m_hp;
};