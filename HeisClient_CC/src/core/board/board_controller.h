/**
*	@file		board_controller.h
*	@brief		heis 盤面操作クラス
*	@author		Ryo Konno
*	@details	盤面上の兵士を操作する。
*/

#pragma once

#include "common_structure.h"
#include "board.h"
#include "board_observer.h"

#include <string>

/**
*	@brief 盤面操作クラス
*/
class CBoardController
{
	// メンバ関数
public:
	// 兵士に攻撃を指示する
	void attack(CBoard* board, const CBoardObserver& observer, const BoardPosition& src, const BoardPosition& dst) const;
	// 兵士に移動を指示する
	void move(CBoard* board, const CBoardObserver& observer, const BoardPosition& src, const BoardPosition& dst) const;
};