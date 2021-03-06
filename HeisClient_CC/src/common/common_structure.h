﻿/**
*	@file		common_structure.h
*	@brief		heis 共通データ構造
*	@author		Ryo Konno
*	@details	プログラム中で共通して使う構造体やクラスを定義する．
*/
#pragma once

#include <cstdint>

/**
*	@struct BoardPosition
*	@brief 盤面上の位置
*/
struct BoardPosition
{	
	// メンバ関数
	public:
		//! コンストラクタ(位置指定あり)
		BoardPosition(const uint16_t init_x,  const uint16_t init_y) 
			: x(init_x)
			, y(init_y)
		{}
		//! コンストラクタ(位置指定なし)
		BoardPosition()
			: x(0)
			, y(0)
		{}
		//! 比較演算子(==)
		bool operator==(const BoardPosition& dst)
		{
			return x == dst.x && y == dst.y;
		}
		//! 比較演算子(!=)
		bool operator!=(const BoardPosition& dst)
		{
			return x != dst.x || y != dst.y;
		}

	// メンバ変数
	public:
		//! x座標
		uint16_t x;
		//! y座標
		uint16_t y;
};