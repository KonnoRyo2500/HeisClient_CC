// heis 司令官クラス
// Author: Ryo Konno
#pragma once

#include "infantry.h"
#include <array>

class CCommander
{
	// 構造体，列挙体など
	private:

	// メンバ関数

	// メンバ変数
	private:
		std::array<CInfantry*, 100> m_infantries;
};