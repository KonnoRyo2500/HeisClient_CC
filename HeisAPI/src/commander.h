// heis 司令官クラス
// Author: Ryo Konno
#pragma once

#include "infantry.h"
#include <vector>

#define INFANTRY_STATUS_ERROR (-1)

class CCommander
{
	// 構造体，列挙体など
	private:

	// メンバ関数
	public:
		// コンストラクタ
		explicit CCommander(const std::string& team_name);

		// ステータスの取得
		int16_t get_x_position(const std::string& id) const;
		int16_t get_y_position(const std::string& id) const;
		int8_t get_action_remain(const std::string& id) const;
		int8_t get_hp(const std::string& id) const;

		// 行動(指定したIDを持つ兵士への命令)
		void attack(const std::string& id, const Direction direction) const;
		void move(const std::string& id, const Direction direction) const;

		// 行動可能な兵士IDの取得
		std::vector<std::string> get_all_movable_infantry_ids() const;

		// 内部処理用(ユーザAIはこの処理を呼ばないこと)
		void update_all_infantries();

	private:
		CInfantry* search_infantry_by_id(const std::string& id) const;
		bool is_movable(const CInfantry* infantry) const;

	// メンバ変数
	private:
		std::string m_team_name;					// チーム名
		std::vector<CInfantry*> m_infantries;		// チームの全兵士
};