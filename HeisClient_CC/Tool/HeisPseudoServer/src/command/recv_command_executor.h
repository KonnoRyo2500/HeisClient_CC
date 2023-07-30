/**
*	@file		recv_command_executor.h
*	@brief		PS recv�R�}���h���s�N���X
*	@author		Ryo Konno
*	@details	PS��recv�R�}���h�����s����B
*/

#pragma once

#include "command_executor_base.h"
#include "socket.h"

/**
*	@brief recv�R�}���h���s�N���X
*/
class CRecvCommandExecutor : public CCommandExecutorBase
{
	// �����o�֐�
public:
	// �R�}���h�����s����
	void execute(const std::vector<std::string>& command);

	// �\�P�b�g��o�^����
	void set_sockets(const CSocket& socket_first, const CSocket& socket_second);

private:
	// JSON����M����
	std::string recv_json(const std::string& from);

	// �I�v�V�����ɉ����āAJSON���o�͂���
	void output_json(const std::vector<std::string>& command, const std::string& json);
	// JSON���t�@�C���ɏo�͂���
	void output_json_to_file(const std::string& filename, const std::string& json);
	// JSON��W���o�͂ɏo�͂���
	void output_json_to_stdout(const std::string& json);

	// �����o�ϐ�
private:
	//! ��U�v���C���[�p�\�P�b�g
	CSocket m_socket_first;
	//! ��U�v���C���[�p�\�P�b�g
	CSocket m_socket_second;
};
