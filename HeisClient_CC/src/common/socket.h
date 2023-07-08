/**
*	@file		socket.h
*	@brief		TCP/IP�\�P�b�g�N���X
*	@author		Ryo Konno
*	@details	�v���b�g�t�H�[���Ɉ˂�Ȃ�TCP/IP�\�P�b�g�����񋟂���B
*/

#pragma once

#include "socket_base.h"

#include <string>

/**
*	@brief	TCP/IP�\�P�b�g�N���X
*/
class CSocket
{
	// �����o�ϐ�
public:
	// �R���X�g���N�^
	explicit CSocket();

	// �\�P�b�g�ɖ��O��t����
	void bind(const uint16_t dst_port, const std::string& src_addr);
	// ���肩��̐ڑ����󂯕t����
	void listen();
	// ���肩��̐ڑ���҂�
	void accept();
	// �\�P�b�g��ڑ�����
	void connect(const std::string& addr, const uint16_t port) const;
	// �f�[�^�𑗐M����
	void send(const std::string& data, const char terminal = '\0');
	// �f�[�^����M����
	std::string recv(const char terminal = '\0');
	// �\�P�b�g�����
	void close();

	// socket��CSocketBase�N���X�̃R���X�g���N�^�ŌĂ΂��
	// ��`���ACSocketBase�N���X���p�������N���X�ł���Ă���

	// �����o�ϐ�
private:
	//! �\�P�b�g�̎���
	CSocketBase* m_socket;
};