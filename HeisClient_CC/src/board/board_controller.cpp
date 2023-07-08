/**
*	@file		board_controller.cpp
*	@brief		heis 盤面操作クラス
*	@author		Ryo Konno
*	@details	盤面上の兵士を操作する。
*/

#include "board_controller.h"

#include <algorithm>

/**
*	@brief 兵士に攻撃を指示する
*	@param[out] board 盤面
*	@param[in] observer 盤面情報取得インスタンス
*	@param[in] src 攻撃元座標
*	@param[in] dst 攻撃先座標
*/
void CBoardController::attack(CBoard* board, const CBoardObserver& observer, const BoardPosition& src, const BoardPosition& dst) const
{
	// 攻撃元の兵士を取得
	Square src_sq = board->get_square(src);
	if (!src_sq.exists) {
		throw std::runtime_error("攻撃元の兵士がいません");
	}

	// 攻撃先の兵士を取得
	Square dst_sq = board->get_square(dst);
	if (!dst_sq.exists) {
		throw std::runtime_error("攻撃先の兵士がいません");
	}

	// 兵士が行動できるか判定
	CInfantry src_infantry = src_sq.infantry;
	uint8_t action_remain = src_infantry.get_status().action_remain;
	if (action_remain == 0) {
		throw std::runtime_error("兵士はもう行動できません");
	}

	// 攻撃可能かどうかを判定
	std::vector<BoardPosition> dsts = observer.search_position_to_attack(*board, src);
	if (std::find(dsts.begin(), dsts.end(), dst) == dsts.end()) {
		throw std::runtime_error("指定された座標に攻撃できません");
	}

	// 攻撃を実施
	CInfantry dst_infantry = dst_sq.infantry;
	dst_infantry.reduce_hp();
	if (dst_infantry.is_dead()) {
		board->remove_infantry(dst);
	}
	else {
		board->set_infantry(dst, dst_infantry);
	}

	// 攻撃した兵士はそれ以上行動できない
	src_infantry.reduce_remaining_action(INFANTRY_ACTION_LIMIT);
	board->set_infantry(src, src_infantry);
}

/**
*	@brief 兵士に移動を指示する
*	@param[out] board 盤面
*	@param[in] observer 盤面情報取得インスタンス
*	@param[in] src 移動元座標
*	@param[in] dst 移動先座標
*/
void CBoardController::move(CBoard* board, const CBoardObserver& observer, const BoardPosition& src, const BoardPosition& dst) const
{
	// 移動元の兵士を取得
	Square src_sq = board->get_square(src);
	if (!src_sq.exists) {
		throw std::runtime_error("移動元の兵士がいません");
	}

	// 移動先の兵士を取得
	Square dst_sq = board->get_square(dst);
	if (dst_sq.exists) {
		throw std::runtime_error("移動先にすでに兵士がいます");
	}

	// 行動回数が足りているか判定
	CInfantry src_infantry = src_sq.infantry;
	uint8_t action_remain = src_infantry.get_status().action_remain;
	if (action_remain < src.calc_l1_distance(dst)) {
		throw std::runtime_error("指定された兵士の行動回数が足りません");
	}

	// 移動可能かどうかを判定
	std::vector<BoardPosition> dsts = observer.search_position_to_move(*board, src);
	if (std::find(dsts.begin(), dsts.end(), dst) == dsts.end()) {
		throw std::runtime_error("指定された座標に攻撃できません");
	}

	// 行動回数を減らす
	src_infantry.reduce_remaining_action(src.calc_l1_distance(dst));

	// 移動を実施
	board->remove_infantry(src);
	board->set_infantry(dst, src_infantry);
}