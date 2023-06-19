/**
*	@file		board_controller.h
*	@brief		heis �Ֆʑ���N���X
*	@author		Ryo Konno
*	@details	�Ֆʏ�̕��m�𑀍삷��B
*/

#pragma once

#include "common_structure.h"
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
	void attack(CBoard* board, CBoardObserver observer, BoardPosition src, BoardPosition dst);
	// ���m�Ɉړ����w������
	void move(CBoard* board, CBoardObserver observer, BoardPosition src, BoardPosition dst);
};