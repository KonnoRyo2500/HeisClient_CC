// heis 各種定数
// Author: Ryo Konno
/**
*	@file		const_val.h
*	@brief		heis 各種定数
*	@author		Ryo Konno
*	@details	プログラム中で共通して使う定数を定義する．
*/
#pragma once

#include<string>

/* 全モード共通で使用する定数 */

/**
*	@enum Direction
*	方向(上下左右)
*/
enum Direction {
	//! 上
	Direction_Up,
	//! 下
	Direction_Down,
	//! 左
	Direction_Left,
	//! 右
	Direction_Right,
};

/**
*	@def 兵士の駒タイプ
*/
#define INFANTRY_UNIT_TYPE "heizu"

/**
*	@def 兵士の初期HP
*/
#define INFANTRY_INITIAL_HP (2)

/**
*	@def 兵士の行動回数
*/
#define INFANTRY_ACTION_LIMIT (2)