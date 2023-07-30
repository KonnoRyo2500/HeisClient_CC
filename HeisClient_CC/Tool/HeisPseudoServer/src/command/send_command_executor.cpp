/**
*	@file		send_command_executor.cpp
*	@brief		PS send�R�}���h���s�N���X
*	@author		Ryo Konno
*	@details	PS��send�R�}���h�����s����B
*/

#include "send_command_executor.h"
#include "path.h"

#include <stdexcept>
#include <fstream>

/**
*	@brief �R�}���h�����s����
*	@param[in] command �R�}���h
*/
void CSendCommandExecutor::execute(const std::vector<std::string>& command)
{
	if (command.size() <= 1) {
		throw std::runtime_error("send�R�}���h: �Œ�1�̃I�v�V�������K�v�ł�");
		return;
	}

	std::string json = load_json(command);
	send_json(command.back(), json);
}

/**
*	@brief �\�P�b�g��o�^����
*	@param[in] socket_first ��U�v���C���[�p�\�P�b�g
*	@param[in] socket_second ��U�v���C���[�p�\�P�b�g
*/
void CSendCommandExecutor::set_sockets(const CSocket& socket_first, const CSocket& socket_second)
{
	m_socket_first = socket_first;
	m_socket_second = socket_second;
}

/**
*	@brief �I�v�V�����ɉ����āAJSON��ǂݍ���
*	@param[in] command �R�}���h
*	@return std::string �ǂݍ���JSON
*/
std::string CSendCommandExecutor::load_json(const std::vector<std::string>& command)
{
	std::string arg_from = command[1];
	std::string json;

	if (arg_from == "file") {
		if (command.size() == 2) {
			throw std::runtime_error("send�R�}���h: file�I�v�V�����ɂ̓t�@�C�������K�v�ł�");
		}
		json = load_json_from_file(command[2]);
	}
	else {
		throw std::runtime_error("send�R�}���h: ����`�̃I�v�V�����ł�");
	}

	return json;
}

/**
*	@brief �t�@�C������JSON��ǂݍ���
*	@param[in] filename �t�@�C����
*	@return std::string �ǂݍ���JSON
*	@remark JSON�t�@�C���́Asetting/files�f�B���N�g������ǂݍ��܂��
*	@remark JSON�́A1�t�@�C���ɂ�1�A1�s�ŏ�����Ă���z��
*/
std::string CSendCommandExecutor::load_json_from_file(const std::string& filename)
{
	// JSON�t�@�C���̃p�X���쐬����
	std::string path = join({
		PS_SETTING_DIR,
		"files",
		filename
	});

	// JSON�t�@�C�����J��
	std::ifstream ifs(path);
	if (ifs.fail()) {
		throw std::runtime_error("send�R�}���h: JSON�t�@�C���̃I�[�v���Ɏ��s���܂���");
	}

	// JSON��ǂݍ���
	std::string json;
	std::getline(ifs, json);

	return json;
}

/**
*	@brief JSON�𑗐M����
*	@param[in] command �R�}���h
*	@param[in] json ���M����JSON
*/
void CSendCommandExecutor::send_json(const std::string& to, const std::string& json)
{
	CSocket target_socket;

	// ���M��̌���
	if (to == "first") {
		target_socket = m_socket_first;
	}
	else if (to == "second") {
		target_socket = m_socket_second;
	}
	else {
		throw std::runtime_error("send�R�}���h: ����`�̑��M��ł�");
	}

	// JSON�𑗐M
	target_socket.send(json);
}
