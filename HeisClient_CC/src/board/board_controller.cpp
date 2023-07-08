/**
*	@file		board_controller.cpp
*	@brief		heis ”Õ–Ê‘€ìƒNƒ‰ƒX
*	@author		Ryo Konno
*	@details	”Õ–Êã‚Ì•ºm‚ğ‘€ì‚·‚éB
*/

#include "board_controller.h"

#include <algorithm>

/**
*	@brief •ºm‚ÉUŒ‚‚ğw¦‚·‚é
*	@param[out] board ”Õ–Ê
*	@param[in] observer ”Õ–Êî•ñæ“¾ƒCƒ“ƒXƒ^ƒ“ƒX
*	@param[in] src UŒ‚Œ³À•W
*	@param[in] dst UŒ‚æÀ•W
*/
void CBoardController::attack(CBoard* board, const CBoardObserver& observer, const BoardPosition& src, const BoardPosition& dst) const
{
	// UŒ‚Œ³‚Ì•ºm‚ğæ“¾
	Square src_sq = board->get_square(src);
	if (!src_sq.exists) {
		throw std::runtime_error("UŒ‚Œ³‚Ì•ºm‚ª‚¢‚Ü‚¹‚ñ");
	}

	// UŒ‚æ‚Ì•ºm‚ğæ“¾
	Square dst_sq = board->get_square(dst);
	if (!dst_sq.exists) {
		throw std::runtime_error("UŒ‚æ‚Ì•ºm‚ª‚¢‚Ü‚¹‚ñ");
	}

	// •ºm‚ªs“®‚Å‚«‚é‚©”»’è
	CInfantry src_infantry = src_sq.infantry;
	uint8_t action_remain = src_infantry.get_status().action_remain;
	if (action_remain == 0) {
		throw std::runtime_error("•ºm‚Í‚à‚¤s“®‚Å‚«‚Ü‚¹‚ñ");
	}

	// UŒ‚‰Â”\‚©‚Ç‚¤‚©‚ğ”»’è
	std::vector<BoardPosition> dsts = observer.search_position_to_attack(*board, src);
	if (std::find(dsts.begin(), dsts.end(), dst) == dsts.end()) {
		throw std::runtime_error("w’è‚³‚ê‚½À•W‚ÉUŒ‚‚Å‚«‚Ü‚¹‚ñ");
	}

	// UŒ‚‚ğÀ{
	CInfantry dst_infantry = dst_sq.infantry;
	dst_infantry.reduce_hp();
	if (dst_infantry.is_dead()) {
		board->remove_infantry(dst);
	}
	else {
		board->set_infantry(dst, dst_infantry);
	}

	// UŒ‚‚µ‚½•ºm‚Í‚»‚êˆÈãs“®‚Å‚«‚È‚¢
	src_infantry.reduce_remaining_action(INFANTRY_ACTION_LIMIT);
	board->set_infantry(src, src_infantry);
}

/**
*	@brief •ºm‚ÉˆÚ“®‚ğw¦‚·‚é
*	@param[out] board ”Õ–Ê
*	@param[in] observer ”Õ–Êî•ñæ“¾ƒCƒ“ƒXƒ^ƒ“ƒX
*	@param[in] src ˆÚ“®Œ³À•W
*	@param[in] dst ˆÚ“®æÀ•W
*/
void CBoardController::move(CBoard* board, const CBoardObserver& observer, const BoardPosition& src, const BoardPosition& dst) const
{
	// ˆÚ“®Œ³‚Ì•ºm‚ğæ“¾
	Square src_sq = board->get_square(src);
	if (!src_sq.exists) {
		throw std::runtime_error("ˆÚ“®Œ³‚Ì•ºm‚ª‚¢‚Ü‚¹‚ñ");
	}

	// ˆÚ“®æ‚Ì•ºm‚ğæ“¾
	Square dst_sq = board->get_square(dst);
	if (dst_sq.exists) {
		throw std::runtime_error("ˆÚ“®æ‚É‚·‚Å‚É•ºm‚ª‚¢‚Ü‚·");
	}

	// s“®‰ñ”‚ª‘«‚è‚Ä‚¢‚é‚©”»’è
	CInfantry src_infantry = src_sq.infantry;
	uint8_t action_remain = src_infantry.get_status().action_remain;
	if (action_remain < src.calc_l1_distance(dst)) {
		throw std::runtime_error("w’è‚³‚ê‚½•ºm‚Ìs“®‰ñ”‚ª‘«‚è‚Ü‚¹‚ñ");
	}

	// ˆÚ“®‰Â”\‚©‚Ç‚¤‚©‚ğ”»’è
	std::vector<BoardPosition> dsts = observer.search_position_to_move(*board, src);
	if (std::find(dsts.begin(), dsts.end(), dst) == dsts.end()) {
		throw std::runtime_error("w’è‚³‚ê‚½À•W‚ÉUŒ‚‚Å‚«‚Ü‚¹‚ñ");
	}

	// s“®‰ñ”‚ğŒ¸‚ç‚·
	src_infantry.reduce_remaining_action(src.calc_l1_distance(dst));

	// ˆÚ“®‚ğÀ{
	board->remove_infantry(src);
	board->set_infantry(dst, src_infantry);
}