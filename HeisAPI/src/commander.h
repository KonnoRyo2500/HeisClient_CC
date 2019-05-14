// heis 司令官クラス
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

		// 行動(指定したIDを持つ兵士への命令)
		void attack(const std::string& id, const Direction direction);
		void move(const std::string& id, const Direction direction);
		void double_move(const std::string& id, const Direction first_move_direction, const Direction second_move_direction);

		// 行動可能な兵士の取得
		std::vector<std::string&> get_all_movable_infantry_ids();

		// 内部処理用(ユーザAIはこの処理を呼ばないこと)
		void update_all_infantries();

	private:
		CInfantry* find_infantry_by_id(const std::string& id);

	// メンバ変数
	private:
		std::string m_team_name;
		std::vector<CInfantry*> m_infantries;		// チームの全兵士
};