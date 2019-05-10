// heis 兵士クラス
// Author: Ryo Konno
#pragma once

#include <string>

class CInfantry
{
	// 構造体，列挙体など
	public:
		// 方向
		enum Direction {
			Direction_Up,					// 上
			Direction_Down,					// 下
			Direction_Left,					// 左
			Direction_Right,				// 右
		};

	private:
		// 初期値
		enum InitialValue {
			InitialValue_HP = 2,			// HP
			InitialValue_ActionNum = 2,		// 行動回数
		};

	// メンバ関数
	public:
		// コンストラクタ
		explicit CInfantry(const std::string& team_name, const uint16_t infantry_ID, const uint16_t init_pos_x, uint16_t init_pos_y);

		// ステータスの取得
		uint8_t get_hp();
		uint16_t get_x_position();
		uint16_t get_y_position();

		// 行動
		void attack(Direction direction);
		void move(Direction direction);

		// 内部処理用(ユーザ定義AIでは呼ばないこと)
		void attacked();

	private:

	// メンバ変数
	private:
		std::string m_team_name;
		uint16_t m_id;
		uint16_t m_pos_x;
		uint16_t m_pos_y;
		uint8_t m_action_remain;
		uint8_t m_hp;
};