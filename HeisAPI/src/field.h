// heis フィールドクラス
// Author: Ryo Konno
#pragma once

#include "infantry.h"
#include <array>

class CField
{
	// 構造体，列挙体など
	private:
		// フィールドに関する定数
		enum FieldParam {
			FieldParam_Width = 20,				// 幅
			FieldParam_Height = 20,				// 高さ
		};

	// メンバ関数
	public:
		static CField* get_instance();
		static void create_field();
		static void delete_field();

		CInfantry* get_infantry(const uint16_t pos_x, const uint16_t pos_y) const;
		void set_infantry(const uint16_t pos_x, const uint16_t pos_y, CInfantry* infantry);
	
	private:
		void initalize();
		bool validate_position(const uint16_t pos_x, const uint16_t pos_y) const;

	// メンバ変数
	private:
		static CField* m_instance;												// インスタンス
		std::array<CInfantry*, FieldParam_Width* FieldParam_Height> m_grid;		// フィールドの各マス目
};