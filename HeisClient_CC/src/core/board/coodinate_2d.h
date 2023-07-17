/**
*	@file		coodinate_2d.h
*	@brief		heis 共通データ構造
*	@author		Ryo Konno
*	@details	プログラム中で共通して使う構造体やクラスを定義する．
*/
#pragma once

#include <cstdint>
#include <cmath>

/**
*	@def INVALID_COODINATE
*	@brief 不正な座標
*/
#define INVALID_COODINATE Coordinate2D(-1, -1)

/**
*	@struct Coordinate2D
*	@brief 盤面上の座標
*/
struct Coordinate2D
{	
	// メンバ関数
public:
	/**
	*	@brief コンストラクタ
	*	@param[in] x x座標
	*	@param[in] y y座標
	*/
	explicit Coordinate2D(const int x,  const int y)
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
	uint32_t calc_l1_distance(const Coordinate2D& dst) const
	{
		return std::abs(this->x - dst.x) + std::abs(this->y - dst.y);
	}
	/**
	*	@brief 比較演算子(==)
	*	@param[in] dst 右辺
	*	@return bool 演算子の演算結果
	*/
	bool operator==(const Coordinate2D& dst) const
	{
		return x == dst.x && y == dst.y;
	}
	/**
	*	@brief 比較演算子(!=)
	*	@param[in] dst 右辺
	*	@return bool 演算子の演算結果
	*/
	bool operator!=(const Coordinate2D& dst) const
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