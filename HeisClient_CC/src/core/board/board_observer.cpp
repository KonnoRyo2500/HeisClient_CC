/**
*	@file		board_observer.cpp
*	@brief		heis �Ֆʏ��擾�N���X
*	@author		Ryo Konno
*	@details	�Ֆʏ�̕��m�Ɋւ���e������擾����B
*/

#include "board_observer.h"

#include <assert.h>
#include <algorithm>

/**
*	@brief �Ֆʏ�̑S���m�ƈʒu���擾����
*	@param[in] board �Ֆ�
*	@return std::vector<InfantryWithPos> �Ֆʏ�̑S���m�Ƃ��̈ʒu
*/
std::vector<InfantryWithPos> CBoardObserver::fetch_all_infantry_and_position(const CBoard& board) const
{
	std::vector<InfantryWithPos> infantries_and_positions;
	BoardSize size = board.get_size();

	for (size_t y = 0; y < size.height; y++) {
		for (size_t x = 0; x < size.width; x++) {
			Square sq = board.get_square(BoardPosition(x, y));
			if (!sq.exists) {
				continue;
			}
			
			BoardPosition pos = BoardPosition(x, y);
			infantries_and_positions.push_back(std::make_pair(sq.infantry, pos));
		}
	}

	return infantries_and_positions;
}

/**
*	@brief �w�肳�ꂽ�ʒu�ɂ��镺�m���ړ��ł���}�X���擾����
*	@param[in] board �Ֆ�
*	@param[in] pos �擾�Ώۂ̕��m�̈ʒu
*	@return std::vector<BoardPosition> �ړ��ł���}�X(�w�肵��ID�̕��m�����Ȃ��ꍇ�͏�ɋ�)
*/
std::vector<BoardPosition> CBoardObserver::search_position_to_move(const CBoard& board, const BoardPosition& pos) const
{
	// pos����L1������2�ȓ��̃}�X�𓾂邽�߂̍���
	std::vector<int> dxs{ 0, -1, 0, 1, -2, -1, 1, 2, -1, 0, 1, 0 };
	std::vector<int> dys{ 2, 1, 1, 1, 0, 0, 0, 0, -1, -1, -1, -2 };
	assert(dxs.size() == dys.size());

	// �ړ��\�ȃ}�X��ǉ����ĕԂ�
	std::vector<BoardPosition> pos_to_move;
	for (size_t i = 0; i < dxs.size(); i++) {
		BoardPosition dst = BoardPosition(pos.x + dxs[i], pos.y + dys[i]);
		if (can_move(board, pos, dst)) {
			pos_to_move.push_back(dst);
		}
	}

	return pos_to_move;
}

/**
*	@brief �w�肳�ꂽ���m���U���ł���}�X���擾����
*	@param[in] board �Ֆ�
*	@param[in] pos �擾�Ώۂ̕��m�̈ʒu
*	@return std::vector<BoardPosition> �U���ł���}�X(�w�肵��ID�̕��m�����Ȃ��ꍇ�͏�ɋ�)
*/
std::vector<BoardPosition> CBoardObserver::search_position_to_attack(const CBoard& board, const BoardPosition& pos) const
{
	// pos�̏㉺���E�̃}�X�𓾂邽�߂̍���
	std::vector<int> dxs{ 0, 0, -1, 1 };
	std::vector<int> dys{ -1, 1, 0, 0 };
	assert(dxs.size() == dys.size());

	std::vector<BoardPosition> pos_to_attack;
	Square src_sq = board.get_square(pos);
	for (size_t i = 0; i < dxs.size(); i++) {
		BoardPosition dst = BoardPosition(pos.x + dxs[i], pos.y + dys[i]);
		if (can_attack(board, pos, dst)) {
			pos_to_attack.push_back(dst);
		}
	}

	return pos_to_attack;
}

/**
*	@brief �w�肳�ꂽ���W���Ֆʂ͈͓̔��ɂ��邩���肷��
*	@param[in] board �Ֆ�
*	@param[in] coord ���W
*	@return coord��board���ɂ��邩
*/
bool CBoardObserver::is_coord_in_board(const CBoard& board, const BoardPosition& coord) const
{
	BoardSize size = board.get_size();
	return
		(coord.x >= 0 && coord.x < size.width) &&
		(coord.y >= 0 && coord.y < size.height);
}

/**
*	@brief �w�肳�ꂽ�}�X�Ɉړ��ł��邩�ǂ����𔻒肷��
*	@param[in] board �Ֆ�
*	@param[in] src �ړ����̃}�X
*	@param[in] dst �ړ���̃}�X
*	@return bool �ړ��ł��邩�ǂ���
*/
bool CBoardObserver::can_move(const CBoard& board, const BoardPosition& src, const BoardPosition& dst) const
{
	// 1: dst���Ֆʏ�ɑ��݂��邱��
	if (!is_coord_in_board(board, dst)) {
		return false;
	}

	// 2: dst�ɕ��m�����Ȃ�����
	Square dst_sq = board.get_square(dst);
	if (dst_sq.exists) {
		return false;
	}

	// 3: src�ɕ��m�����邱��
	Square src_sq = board.get_square(src);
	if (!src_sq.exists) {
		return false;
	}

	// 4: ���m�̈ړ��񐔂�����Ă��邱��
	uint8_t action_remain = src_sq.infantry.get_status().action_remain;
	if (action_remain < src.calc_l1_distance(dst)) {
		return false;
	}

	// 5: src����dst�Ɉړ����邽�߂̃p�X�����݂��邱��
	if (!exists_path(board, src, dst)) {
		return false;
	}

	// ��L1?5�����ׂĖ������Ƃ��A�ړ��\
	return true;
}

/**
*	@brief �w�肳�ꂽ�}�X�Ɉړ����邽�߂̃p�X�����݂��邩�ǂ������肷��
*	@details src���N�_�ɔՖʏ�̃}�X���㉺���E�ɂ��ǂ��Ă����Adst�ɂ��ǂ蒅����΃p�X�����݂���B�������A�r���ɓG������ꍇ�͐�ɐi�߂��A�����͒ʂ蔲���\
*	@param[in] board �Ֆ�
*	@param[in] infantry ���m�̎���
*	@param[in] src �ړ����̃}�X
*	@param[in] dst �ړ���̃}�X
*	@return bool src����dst�Ɏ���p�X�����݂��邩�ǂ���
*/
bool CBoardObserver::exists_path(const CBoard& board, const BoardPosition& src, const BoardPosition& dst) const
{
	Square src_sq = board.get_square(src);
	Square dst_sq = board.get_square(dst);
	// src�ɕ��m�����Ȃ�
	if (!src_sq.exists) {
		return false;
	}
	// dst�ɂ��łɕ��m������
	if (dst_sq.exists) {
		return false;
	}

	std::vector<BoardPosition> searching_coords{ src };
	std::vector<BoardPosition> searched_coords{ src };

	// dst�ɓ��B����܂ŁA�p�X��T��
	int max_iteration = src_sq.infantry.get_status().action_remain;
	for (int i = 0; i < max_iteration; i++) {
		std::vector<BoardPosition> next_searching_coords;
		for (auto& sc : searching_coords) {
			// sc�̏㉺���E��T��
			std::vector<BoardPosition> around_coords{
				BoardPosition(sc.x, sc.y - 1),
				BoardPosition(sc.x, sc.y + 1),
				BoardPosition(sc.x - 1, sc.y),
				BoardPosition(sc.x + 1, sc.y)
			};

			for (auto& ac : around_coords) {
				// ac�͂��łɒT���ς�
				if (std::find(searched_coords.begin(), searched_coords.end(), ac) != searched_coords.end()) {
					continue;
				}
				// ac���Ֆʂ͈̔͊O
				if (!is_coord_in_board(board, ac)) {
					continue;
				}
				// dst�Ɏ���p�X����������
				if (ac == dst) {
					return true;
				}
				// ac���󂫃}�X�A�������͖����̂���}�X�ł���Ύ����searching_coords�Ƃ���
				Square ac_sq = board.get_square(ac);
				if (!ac_sq.exists || ac_sq.infantry.get_status().team_name == src_sq.infantry.get_status().team_name) {
					next_searching_coords.push_back(ac);
				}
				
				// ac��T���ς݃}�X�ɉ�����
				searched_coords.push_back(ac);
			}
		}

		// �T�������}�X���N�_�ɁA����̃p�X�T�����s��
		searching_coords = next_searching_coords;
	}

	// dst�Ɏ���p�X��������Ȃ�����
	return false;
}

/**
*	@brief �w�肳�ꂽ�}�X�ɍU���ł��邩�ǂ������肷��
*	@param[in] board �Ֆ�
*	@param[in] src �ړ����̃}�X
*	@param[in] dst �ړ���̃}�X
*	@return bool �U���ł��邩�ǂ���
*/
bool CBoardObserver::can_attack(const CBoard& board, const BoardPosition& src, const BoardPosition& dst) const
{
	// 1: dst���Ֆʏ�ɑ��݂��邱��
	if (!is_coord_in_board(board, dst)) {
		return false;
	}

	// 2: dst�ɕ��m�����邱��
	Square dst_sq = board.get_square(dst);
	if (!dst_sq.exists) {
		return false;
	}

	// 3: src�ɕ��m�����邱��
	Square src_sq = board.get_square(src);
	if (!src_sq.exists) {
		return false;
	}

	// 4: ���m�̈ړ��񐔂�����Ă��邱��
	uint8_t action_remain = src_sq.infantry.get_status().action_remain;
	if (action_remain == 0) {
		return false;
	}

	// 5: src�̕��m�ƁAdst�̕��m�̃`�[�������Ⴄ����
	std::string src_team = src_sq.infantry.get_status().team_name;
	std::string dst_team = dst_sq.infantry.get_status().team_name;
	if (src_team == dst_team) {
		return false;
	}

	// ��L1?5�����ׂĖ������Ƃ��A�U���\
	return true;
}