/**
*	@file		send_command_executor.h
*	@brief		PS send�R�}���h���s�N���X
*	@author		Ryo Konno
*	@details	PS��send�R�}���h�����s����B
*/

#pragma once

#include "command_executor_base.h"
#include "socket.h"

/**
*	@brief send�R�}���h���s�N���X
*/
class CSendCommandExecutor : public CCommandExecutorBase
{
	// �����o�֐�
public:
	// �R�}���h�����s����
	void execute(const std::vector<std::string>& command);

	// �\�P�b�g��o�^����
	void set_sockets(const CSocket& socket_first, const CSocket& socket_second);

private:
	// �I�v�V�����ɉ����āAJSON��ǂݍ���
	std::string load_json(const std::vector<std::string>& command);
	// �t�@�C������JSON��ǂݍ���
	std::string load_json_from_file(const std::string& path);

	// JSON�𑗐M����
	void send_json(const std::string& to, const std::string& json);

	// �����o�ϐ�
private:
	//! ��U�v���C���[�p�\�P�b�g
	CSocket m_socket_first;
	//! ��U�v���C���[�p�\�P�b�g
	CSocket m_socket_second;
};
