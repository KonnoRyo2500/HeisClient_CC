/**
*	@file		command_executor_factory.cpp
*	@brief		PS �R�}���h���s�t�@�N�g���[�N���X
*	@author		Ryo Konno
*	@details	PS�̊e��R�}���h�����s����N���X�̃C���X�^���X�𐶐�����B
*/

#include "command_executor_factory.h"
#include "recv_command_executor.h"
#include "send_command_executor.h"

/**
*   @brief �R�}���h���s�N���X�̃C���X�^���X�𐶐�����
*	@param[in] command_name �R�}���h��
*	@param[in] socket_first ��U�v���C���[�p�\�P�b�g
*	@param[in] socket_second ��U�v���C���[�p�\�P�b�g
*	@return CCommandExecutorBase* �R�}���h���s�N���X�̃A�h���X(����`�̃R�}���h�̏ꍇ��NULL)
*/
CCommandExecutorBase* CCommandExecutorFactory::create_instance(
	const std::string& command_name,
	const CSocket& socket_first,
	const CSocket& socket_second
) const
{
	CCommandExecutorBase *instance_addr;

	if (command_name == "recv") {
		CRecvCommandExecutor* recv_executor = new CRecvCommandExecutor();
		recv_executor->set_sockets(socket_first, socket_second);
		instance_addr = (CCommandExecutorBase*)recv_executor;
	}
	else if (command_name == "send") {
		CSendCommandExecutor* send_executor = new CSendCommandExecutor();
		send_executor->set_sockets(socket_first, socket_second);
		instance_addr = (CCommandExecutorBase*)send_executor;
	}
	else {
		instance_addr = NULL;
	}

	return instance_addr;
}
