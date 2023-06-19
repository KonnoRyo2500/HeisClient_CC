/**
*	@file		ai_factory.h
*	@brief		heis AI�t�@�N�g���[�N���X
*	@author		Ryo Konno
*	@details	�e��AI�C���X�^���X�𐶐����邽�߂̃t�@�N�g���[�N���X�B
*/

#pragma once

#include "ai_base.h"

#include <map>

/**
*	@brief	AI�t�@�N�g���[�N���X
*/
class CAIFactory
{
// �����o�֐�
public:
	// AI�C���X�^���X�𐶐�����
	CAIBase* create_instance(CCommander commander, std::string ai_impl_name) const;
};