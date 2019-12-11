// heis 共通データ構造
// Author: Ryo Konno
#pragma once

#include <cstdint>

// プログラム中で共通して使う構造体やクラスをここに記述する

// フィールド上の位置
struct FieldPosition
{	
	// メンバ関数
	public:
		// コンストラクタ
		FieldPosition(const uint16_t init_x,  const uint16_t init_y) 
			: x(init_x)
			, y(init_y)
		{}
		FieldPosition()
			: x(0)
			, y(0)
		{}
		// 比較演算子
		bool operator==(const FieldPosition& dst)
		{
			return x == dst.x && y == dst.y;
		}
		bool operator!=(const FieldPosition& dst)
		{
			return x != dst.x || y != dst.y;
		}

	// メンバ変数
	public:
		uint16_t x;
		uint16_t y;
};