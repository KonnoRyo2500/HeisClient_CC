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
		explicit CCommander(std::string team_name);

		// 内部処理用(ユーザAIはこの処理を呼ばないこと)
		void update_all_infantries();

	private:
		int search_infantry_by_id(uint16_t id);

	// メンバ変数
	private:
		std::string m_team_name;
		std::vector<CInfantry*> m_infantries;		// チームの全兵士
};