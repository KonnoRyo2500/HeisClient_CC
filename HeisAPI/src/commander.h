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

		// 行動(指定したIDを持つ兵士への命令)

		// ユーザAI作成支援用
		std::vector<CInfantry*> get_all_movable_infantries();

		// 内部処理用(ユーザAIはこの処理を呼ばないこと)
		void update_all_infantries();

	private:
		int search_infantry_by_id(const std::string& id);

	// メンバ変数
	private:
		std::string m_team_name;
		std::vector<CInfantry*> m_infantries;		// チームの全兵士
};