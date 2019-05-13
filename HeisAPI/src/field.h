﻿// heis フィールドクラス
// Author: Ryo Konno
#pragma once

#include "infantry.h"
#include <array>

class CField
{
	// メンバ関数
	public:
		// フィールドの取得，作成，削除
		static CField* get_instance();
		static void create_field();
		static void delete_field();

		// マス目情報(各マスにいる兵士の情報)の管理
		CInfantry* get_infantry(const uint16_t pos_x, const uint16_t pos_y) const;
		void set_infantry(const uint16_t pos_x, const uint16_t pos_y, CInfantry* infantry);
	
		// 更新
		void update_field();

	private:
		void initalize();
		void validate_position(const uint16_t pos_x, const uint16_t pos_y) const;

	// メンバ変数
	private:
		static CField* m_instance;												// インスタンス
		std::array<CInfantry*, FieldParam_Width * FieldParam_Height> m_grid;	// フィールドの各マス目
};