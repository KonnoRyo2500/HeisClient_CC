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
		CInfantry* get_infantry(const uint16_t pos_x, const uint16_t pos_y) const;
		void set_infantry(const uint16_t pos_x, const uint16_t pos_y, CInfantry* infantry);
	
		// 内部処理用(ユーザAIはこの処理を呼ばないこと)
		void update_field();

		// デバッグ，動作確認用
		void show();

	private:
		void initalize();
		void validate_position(const uint16_t pos_x, const uint16_t pos_y) const;

	// メンバ変数
	private:
		static CField* m_instance;												// インスタンス
		// TODO: ID(std::string)のvectorにする(今の実装だと，コマの種類が増えた時に対応できない)
		// TODO: コマの実体のvectorを作成する(IDのvectorから実体が取れる)
		std::array<CInfantry*, FieldParam_Width * FieldParam_Height> m_grid;	// フィールドの各マス目
};