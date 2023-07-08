/**
*	@file		ai_base.h
*	@brief		heis AI�x�[�X�N���X
*	@author		Ryo Konno
*	@details	heis�ΐ�AI�̃x�[�X�N���X�B
*/

#pragma once

#include "commander.h"

/**
*	@brief	AI�x�[�X�N���X
*	@detail	heis�ΐ�AI�̃x�[�X�N���X�B���[�U�́A���̃N���X���p������AI����������
*	@remark ���̃N���X�̓C���X�^���X���ł��Ȃ�(���ۃN���X)
*/
class CAIBase
{
// �����o�֐�
public:
	// �R���X�g���N�^
	explicit CAIBase(const CCommander& commander);

	// ���C������
	virtual void AI_main(const JSONRecvPacket_Board& board_pkt) = 0;

// �����o�ϐ�
protected:
	//! ���m�𑀍삷�邽�߂̎i�ߊ�
	CCommander m_commander;
};