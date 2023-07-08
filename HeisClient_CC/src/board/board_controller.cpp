/**
*	@file		board_controller.cpp
*	@brief		heis �Ֆʑ���N���X
*	@author		Ryo Konno
*	@details	�Ֆʏ�̕��m�𑀍삷��B
*/

#include "board_controller.h"

#include <algorithm>

/**
*	@brief ���m�ɍU�����w������
*	@param[out] board �Ֆ�
*	@param[in] observer �Ֆʏ��擾�C���X�^���X
*	@param[in] src �U�������W
*	@param[in] dst �U������W
*/
void CBoardController::attack(CBoard* board, const CBoardObserver& observer, const BoardPosition& src, const BoardPosition& dst) const
{
	// �U�����̕��m���擾
	Square src_sq = board->get_square(src);
	if (!src_sq.exists) {
		throw std::runtime_error("�U�����̕��m�����܂���");
	}

	// �U����̕��m���擾
	Square dst_sq = board->get_square(dst);
	if (!dst_sq.exists) {
		throw std::runtime_error("�U����̕��m�����܂���");
	}

	// ���m���s���ł��邩����
	CInfantry src_infantry = src_sq.infantry;
	uint8_t action_remain = src_infantry.get_status().action_remain;
	if (action_remain == 0) {
		throw std::runtime_error("���m�͂����s���ł��܂���");
	}

	// �U���\���ǂ����𔻒�
	std::vector<BoardPosition> dsts = observer.search_position_to_attack(*board, src);
	if (std::find(dsts.begin(), dsts.end(), dst) == dsts.end()) {
		throw std::runtime_error("�w�肳�ꂽ���W�ɍU���ł��܂���");
	}

	// �U�������{
	CInfantry dst_infantry = dst_sq.infantry;
	dst_infantry.reduce_hp();
	if (dst_infantry.is_dead()) {
		board->remove_infantry(dst);
	}
	else {
		board->set_infantry(dst, dst_infantry);
	}

	// �U���������m�͂���ȏ�s���ł��Ȃ�
	src_infantry.reduce_remaining_action(INFANTRY_ACTION_LIMIT);
	board->set_infantry(src, src_infantry);
}

/**
*	@brief ���m�Ɉړ����w������
*	@param[out] board �Ֆ�
*	@param[in] observer �Ֆʏ��擾�C���X�^���X
*	@param[in] src �ړ������W
*	@param[in] dst �ړ�����W
*/
void CBoardController::move(CBoard* board, const CBoardObserver& observer, const BoardPosition& src, const BoardPosition& dst) const
{
	// �ړ����̕��m���擾
	Square src_sq = board->get_square(src);
	if (!src_sq.exists) {
		throw std::runtime_error("�ړ����̕��m�����܂���");
	}

	// �ړ���̕��m���擾
	Square dst_sq = board->get_square(dst);
	if (dst_sq.exists) {
		throw std::runtime_error("�ړ���ɂ��łɕ��m�����܂�");
	}

	// �s���񐔂�����Ă��邩����
	CInfantry src_infantry = src_sq.infantry;
	uint8_t action_remain = src_infantry.get_status().action_remain;
	if (action_remain < src.calc_l1_distance(dst)) {
		throw std::runtime_error("�w�肳�ꂽ���m�̍s���񐔂�����܂���");
	}

	// �ړ��\���ǂ����𔻒�
	std::vector<BoardPosition> dsts = observer.search_position_to_move(*board, src);
	if (std::find(dsts.begin(), dsts.end(), dst) == dsts.end()) {
		throw std::runtime_error("�w�肳�ꂽ���W�ɍU���ł��܂���");
	}

	// �s���񐔂����炷
	src_infantry.reduce_remaining_action(src.calc_l1_distance(dst));

	// �ړ������{
	board->remove_infantry(src);
	board->set_infantry(dst, src_infantry);
}