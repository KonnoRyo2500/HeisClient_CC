/**
*	@file		board_controller.h
*	@brief		heis �Ֆʑ���N���X
*	@author		Ryo Konno
*	@details	�Ֆʏ�̕��m�𑀍삷��B
*/

#pragma once

#include "coodinate_2d.h"
#include "board.h"
#include "board_observer.h"

#include <string>

/**
*	@brief �Ֆʑ���N���X
*/
class CBoardController
{
	// �����o�֐�
public:
	// ���m�ɍU�����w������
	void attack(CBoard* board, const CBoardObserver& observer, const Coordinate2D& src, const Coordinate2D& dst) const;
	// ���m�Ɉړ����w������
	void move(CBoard* board, const CBoardObserver& observer, const Coordinate2D& src, const Coordinate2D& dst) const;
};