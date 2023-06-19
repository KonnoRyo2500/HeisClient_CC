/**
*	@file		common_structure.h
*	@brief		heis 共通データ構造
*	@author		Ryo Konno
*	@details	プログラム中で共通して使う構造体やクラスを定義する．
*/
#pragma once

#include <cstdint>
#include <cmath>

/**
*	@def INVALID_POSITION
*	@brief 不正な座標
*/
#define INVALID_POSITION BoardPosition(-1, -1)

/**
*	@struct BoardPosition
*	@brief 盤面上の位置
*/
struct BoardPosition
{	
	// メンバ関数
public:
	/**
	*	@brief コンストラクタ
	*	@param[in] x x座標
	*	@param[in] y y座標
	*/
	explicit BoardPosition(const int x,  const int y) 
		: x(x)
		, y(y)
	{
		// Do Nothing
	}
	/**
	*	@brief 指定された点とのL1距離を計算する
	*	@param[in] dst L1座標を計算する点
	*	@return uint32_t 自身とdstとのL1座標
	*/
	uint32_t calc_l1_distance(BoardPosition dst)
	{
		return std::abs(this->x - dst.x) + std::abs(this->y - dst.y);
	}
	/**
	*	@brief 比較演算子(==)
	*	@param[in] dst 右辺
	*/
	bool operator==(const BoardPosition& dst)
	{
		return x == dst.x && y == dst.y;
	}
	/**
	*	@brief 比較演算子(!=)
	*	@param[in] dst 右辺
	*/
	bool operator!=(const BoardPosition& dst)
	{
		return x != dst.x || y != dst.y;
	}

	// メンバ変数
public:
	//! x座標
	int x;
	//! y座標
	int y;
};