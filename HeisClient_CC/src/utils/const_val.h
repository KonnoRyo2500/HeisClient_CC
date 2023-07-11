// heis �e��萔
// Author: Ryo Konno
/**
*	@file		const_val.h
*	@brief		heis �e��萔
*	@author		Ryo Konno
*	@details	�v���O�������ŋ��ʂ��Ďg���萔���`����D
*/
#pragma once

#include<string>

/* �S���[�h���ʂŎg�p����萔 */

/**
*	@enum Direction
*	����(�㉺���E)
*/
enum Direction {
	//! ��
	Direction_Up,
	//! ��
	Direction_Down,
	//! ��
	Direction_Left,
	//! �E
	Direction_Right,
};

/**
*	@def ���m�̋�^�C�v
*/
#define INFANTRY_UNIT_TYPE "heizu"

/**
*	@def ���m�̏���HP
*/
#define INFANTRY_INITIAL_HP (2)

/**
*	@def ���m�̍s����
*/
#define INFANTRY_ACTION_LIMIT (2)