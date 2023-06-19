/**
*	@file		board_observer.h
*	@brief		heis 盤面情報取得クラス
*	@author		Ryo Konno
*	@details	盤面上の兵士に関する各種情報を取得する。
*/

#pragma once

#include "common_structure.h"
#include "board.h"

#include <vector>
#include <string>

/**
*	@brief 兵士の実体と場所を表す型
*/
typedef std::pair<CInfantry, BoardPosition> InfantryWithPos;

/**
*	@brief 盤面情報取得クラス
*/
class CBoardObserver
{
	// メンバ関数
public:
	// 盤面上の全兵士を取得する
	std::vector<InfantryWithPos> fetch_all_infantry_and_position(const CBoard& board) const;

	// 指定された兵士が移動できるマスを取得する
	std::vector<BoardPosition> search_position_to_move(const CBoard& board, const BoardPosition& pos) const;
	// 指定された兵士が攻撃できるマスを取得する
	std::vector<BoardPosition> search_position_to_attack(const CBoard& board, const BoardPosition& pos) const;

private:
	// 指定された座標が盤面の範囲内にあるか判定する
	bool is_coord_in_board(const CBoard& board, const BoardPosition& coord) const;

	// 指定されたマスに移動できるかどうかを判定する
	bool can_move(const CBoard& board, const BoardPosition& src, const BoardPosition& dst) const;
	// 指定されたマスに移動するためのパスが存在するかどうか判定する
	bool exists_path(const CBoard& board, const BoardPosition& src, const BoardPosition& dst) const;

	// 指定されたマスに攻撃できるかどうか判定する
	bool can_attack(const CBoard& board, const BoardPosition& src, const BoardPosition& dst) const;
};