/**
*	@file		board_observer.h
*	@brief		heis �Ֆʏ��擾�N���X
*	@author		Ryo Konno
*	@details	�Ֆʏ�̕��m�Ɋւ���e������擾����B
*/

#pragma once

#include "common_structure.h"
#include "board.h"

#include <vector>
#include <string>

/**
*	@brief ���m�̎��̂Əꏊ��\���^
*/
typedef std::pair<CInfantry, BoardPosition> InfantryWithPos;

/**
*	@brief �Ֆʏ��擾�N���X
*/
class CBoardObserver
{
	// �����o�֐�
public:
	// �Ֆʏ�̑S���m���擾����
	std::vector<InfantryWithPos> fetch_all_infantry_and_position(CBoard board);

	// �w�肳�ꂽ���m���ړ��ł���}�X���擾����
	std::vector<BoardPosition> search_position_to_move(CBoard board, BoardPosition pos);
	// �w�肳�ꂽ���m���U���ł���}�X���擾����
	std::vector<BoardPosition> search_position_to_attack(CBoard board, BoardPosition pos);

private:
	// �w�肳�ꂽ���W���Ֆʂ͈͓̔��ɂ��邩���肷��
	bool is_coord_in_board(CBoard board, BoardPosition coord);

	// �w�肳�ꂽ�}�X�Ɉړ��ł��邩�ǂ����𔻒肷��
	bool can_move(CBoard board, BoardPosition src, BoardPosition dst);
	// �w�肳�ꂽ�}�X�Ɉړ����邽�߂̃p�X�����݂��邩�ǂ������肷��
	bool exists_path(CBoard board, BoardPosition src, BoardPosition dst);

	// �w�肳�ꂽ�}�X�ɍU���ł��邩�ǂ������肷��
	bool can_attack(CBoard board, BoardPosition src, BoardPosition dst);
};