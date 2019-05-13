// heis 司令官クラス
// Author: Ryo Konno
#pragma once

#include "infantry.h"
#include <array>

class CCommander
{
	// 構造体，列挙体など
	private:
		// 初期値
		enum InitialValue {
			InitialValue_NumInfantry = 30,			// 兵士の人数
		};

	// メンバ関数
	public:
		// コンストラクタ
		explicit CCommander(std::string team_name);

	// メンバ変数
	private:
		std::string m_team_name;
		std::array<CInfantry*, InitialValue_NumInfantry> m_infantries;		// チームの全兵士
};