/**
*	@file		command_executor_base.h
*	@brief		PS �R�}���h���s�x�[�X�N���X
*	@author		Ryo Konno
*	@details	PS�̊e��R�}���h�����s����N���X�̊�{�N���X�B
*/

#pragma once

#include <string>
#include <vector>

/**
*	@brief �R�}���h���s�x�[�X�N���X
*/
class CCommandExecutorBase
{
	// �����o�֐�
public:
	// �R�}���h�����s����
	virtual void execute(const std::vector<std::string>& command) = 0;
};
