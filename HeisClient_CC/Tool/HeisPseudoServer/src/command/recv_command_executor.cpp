/**
*	@file		recv_command_executor.cpp
*	@brief		PS recv�R�}���h���s�N���X
*	@author		Ryo Konno
*	@details	PS��recv�R�}���h�����s����B
*/

#include "recv_command_executor.h"
#include "path.h"

#include <stdexcept>
#include <fstream>

/**
*	@brief �R�}���h�����s����
*	@param[in] command �R�}���h
*/
void CRecvCommandExecutor::execute(const std::vector<std::string>& command)
{
	if (command.size() <= 1) {
		throw std::runtime_error("recv�R�}���h: �Œ�1�̃I�v�V�������K�v�ł�");
		return;
	}

	std::string json = recv_json(command.back());
	output_json(command, json);
}

/**
*	@brief �\�P�b�g��o�^����
*	@param[in] socket_first ��U�v���C���[�p�\�P�b�g
*	@param[in] socket_second ��U�v���C���[�p�\�P�b�g
*/
void CRecvCommandExecutor::set_sockets(const CSocket& socket_first, const CSocket& socket_second)
{
	m_socket_first = socket_first;
	m_socket_second = socket_second;
}

/**
*	@brief JSON����M����
*	@param[in] from ��M��
*	@return std::string ��M����JSON
*/
std::string CRecvCommandExecutor::recv_json(const std::string& from)
{
	CSocket target_socket;

	// ��M���̌���
	if (from == "first") {
		target_socket = m_socket_first;
	}
	else if (from == "second") {
		target_socket = m_socket_second;
	}
	else {
		throw std::runtime_error("recv�R�}���h: ����`�̎�M���ł�");
	}

	// JSON����M
	std::string json = target_socket.recv();

	return json;
}

/**
*	@brief �I�v�V�����ɉ����āAJSON���o�͂���
*	@param[in] command �R�}���h
*	@param[in] json JSON
*/
void CRecvCommandExecutor::output_json(const std::vector<std::string>& command, const std::string& json)
{
	std::string arg_to = command[1];

	if (arg_to == "file") {
		if (command.size() == 2) {
			throw std::runtime_error("recv�R�}���h: file�I�v�V�����ɂ̓t�@�C�������K�v�ł�");
		}
		output_json_to_file(command[2], json);
	}
	else if (arg_to == "print") {
		output_json_to_stdout(json);
	}
	else {
		throw std::runtime_error("recv�R�}���h: ����`�̃I�v�V�����ł�");
	}
}

/**
*	@brief JSON���t�@�C���ɏo�͂���
*	@param[in] filename �o�̓t�@�C����
*	@param[in] json JSON
*	@remark JSON�t�@�C���́Asetting/files�f�B���N�g������ǂݍ��܂��
*	@remark JSON�́A1�t�@�C���ɂ�1�A1�s�ŏo�͂����z��
*/
void CRecvCommandExecutor::output_json_to_file(const std::string& filename, const std::string& json)
{
	// JSON�t�@�C���̃p�X���쐬����
	std::string path = join({
		PS_SETTING_DIR,
		"files",
		filename
	});

	// JSON�t�@�C�����J��
	std::ofstream ofs(path);
	if (ofs.fail()) {
		throw std::runtime_error("recv�R�}���h: JSON�t�@�C���̃I�[�v���Ɏ��s���܂���");
	}

	// JSON���t�@�C���ɏ�������
	ofs.write(json.c_str(), json.size());
}

/**
*	@brief JSON��W���o�͂ɏo�͂���
*	@param[in] json JSON
*/
void CRecvCommandExecutor::output_json_to_stdout(const std::string& json)
{
	printf("%s\n", json.c_str());
}
