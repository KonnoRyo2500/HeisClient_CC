/**
*	@file		infantry.cpp
*	@brief		heis ���m�N���X
*	@author		Ryo Konno
*	@details	heis�̃��j�b�g��1�ł��镺�m���j�b�g���`����D
*/
#include "infantry.h"
#include "board.h"
#include <stdexcept>
#include <queue>

/* public�֐� */

/**
*	@brief �R���X�g���N�^
*	@param[in] team_name �����`�[����
*	@param[in] id ID
*	@param[in] hp ����HP(�u�ՖʁvJSON����Ĕz�u����ۂɎg�p����)
*/
CInfantry::CInfantry(const InfantryStatus& init_status)
	: m_status(init_status)
{
	// Do Nothing
}

/**
*	@brief �f�X�g���N�^
*/
CInfantry::~CInfantry()
{
	// Do Nothing
}

/**
*	@brief ���݂̃X�e�[�^�X���擾����
*	@return InfantryStatus ���݂̃X�e�[�^�X
*/
InfantryStatus CInfantry::get_status() const
{
	return m_status;
}

/**
*	@brief HP��1���炷
*	@remark ���ł�HP��0�̏ꍇ�͉������Ȃ�
*/
void CInfantry::reduce_hp()
{
	if (m_status.hp >= 1) {
		m_status.hp--;
	}
}

/**
*	@brief �s���񐔂��w�肳�ꂽ�񐔌��炷
*	@param[in] n_reduce �s���񐔂̌�����
*	@remark �s���񐔂�0�ȉ��ɂ͂Ȃ�Ȃ�
*/
void CInfantry::reduce_remaining_action(uint8_t n_reduce)
{
	if (m_status.action_remain >= n_reduce) {
		m_status.action_remain -= n_reduce;
	}
	else {
		m_status.action_remain = 0;
	}
}

/**
*	@brief ����������ł��邩�𔻒肷��
*	@return bool ����������ł��邩
*/
bool CInfantry::is_dead() const
{
	return m_status.hp == 0;
}