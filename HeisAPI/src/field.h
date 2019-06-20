// heis フィールドクラス
// Author: Ryo Konno
#pragma once

#include "infantry.h"
#include <array>

// ユーザAIはこのクラスの関数を呼ばないこと
class CField
{
	// 構造体，列挙体など
	public:
		
	// メンバ関数
	public:
		// フィールドの取得，作成，削除
		static CField* get_instance();
		static void create_field();
		static void delete_field();

		// マス目情報(各マスにいる兵士の情報)の管理
		CInfantry* get_infantry(const int16_t pos_x, const int16_t pos_y) const;
		void set_infantry(const int16_t pos_x, const int16_t pos_y, CInfantry* infantry);
	
		// 内部処理用(ユーザAIはこの処理を呼ばないこと)
		void update_field();

		// デバッグ，動作確認用
		void show();

	private:
		void initalize();
		void validate_position(const int16_t pos_x, const int16_t pos_y) const;

	// メンバ変数
	private:
		static CField* m_instance;												// インスタンス
		std::array<CInfantry*, FieldParam_Width * FieldParam_Height> m_grid;	// フィールドの各マス目
};