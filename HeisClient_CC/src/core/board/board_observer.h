/**
*	@file		board_observer.h
*	@brief		heis �Ֆʏ��擾�N���X
*	@author		Ryo Konno
*	@details	�Ֆʏ�̕��m�Ɋւ���e������擾����B
*/

#pragma once

#include "coodinate_2d.h"
#include "board.h"

#include <vector>
#include <string>

/**
*	@brief ���m�̎��̂Əꏊ��\���^
*/
typedef std::pair<CInfantry, Coordinate2D> InfantryWithPos;

/**
*	@brief �Ֆʏ��擾�N���X
*/
class CBoardObserver
{
	// �����o�֐�
public:
	// �Ֆʏ�̑S���m���擾����
	std::vector<InfantryWithPos> fetch_all_infantry_and_position(const CBoard& board) const;

	// �w�肳�ꂽ���m���ړ��ł���}�X���擾����
	std::vector<Coordinate2D> search_position_to_move(const CBoard& board, const Coordinate2D& pos) const;
	// �w�肳�ꂽ���m���U���ł���}�X���擾����
	std::vector<Coordinate2D> search_position_to_attack(const CBoard& board, const Coordinate2D& pos) const;

private:
	// �w�肳�ꂽ���W���Ֆʂ͈͓̔��ɂ��邩���肷��
	bool is_coord_in_board(const CBoard& board, const Coordinate2D& coord) const;

	// �w�肳�ꂽ�}�X�Ɉړ��ł��邩�ǂ����𔻒肷��
	bool can_move(const CBoard& board, const Coordinate2D& src, const Coordinate2D& dst) const;
	// �w�肳�ꂽ�}�X�Ɉړ����邽�߂̃p�X�����݂��邩�ǂ������肷��
	bool exists_path(const CBoard& board, const Coordinate2D& src, const Coordinate2D& dst) const;

	// �w�肳�ꂽ�}�X�ɍU���ł��邩�ǂ������肷��
	bool can_attack(const CBoard& board, const Coordinate2D& src, const Coordinate2D& dst) const;
};