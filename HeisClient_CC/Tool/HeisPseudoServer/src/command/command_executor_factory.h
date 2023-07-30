/**
*	@file		command_executor_factory.h
*	@brief		PS �R�}���h���s�t�@�N�g���[�N���X
*	@author		Ryo Konno
*	@details	PS�̊e��R�}���h�����s����N���X�̃C���X�^���X�𐶐�����B
*/

#pragma once

#include "command_executor_base.h"
#include "socket.h"

/**
*	@brief �R�}���h���s�t�@�N�g���[�N���X
*/
class CCommandExecutorFactory
{
	// �����o�֐�
public:
	// �R�}���h���s�N���X�̃C���X�^���X�𐶐�����
	CCommandExecutorBase* create_instance(
		const std::string& command_name,
		const CSocket& socket_first,
	const CSocket& socket_second
	) const;
};
