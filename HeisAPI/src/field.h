// heis フィールドクラス
// Author: Ryo Konno
#pragma once

#include "infantry.h"
#include <array>

// TODO: シングルトンパターンをこのクラスに適用する
class CField
{
	// 構造体，列挙体など
	private:
		// フィールドに関する定数
		enum FieldParam {
			FieldParam_Width = 20,				// 幅
			FieldParam_Height = 20,				// 高さ
		};

	// メンバ変数
	private:
		std::array<CInfantry*, FieldParam_Width * FieldParam_Height> m_field;		// フィールドの各マス目

	// メンバ関数
	public:
		CInfantry* get_infantry(uint16_t pos_x, uint16_t pos_y);
		void set_infantry(uint16_t pos_x, uint16_t pos_y, CInfantry* infantry);
		void end_your_turn();
};