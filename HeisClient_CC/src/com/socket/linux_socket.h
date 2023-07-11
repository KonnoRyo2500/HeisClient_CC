/**
*	@file		linux_socket.h
*	@brief		Linux�pTCP/IP�\�P�b�g�N���X
*	@author		Ryo Konno
*	@details	Linux���ɂ�����\�P�b�g�̊e�푀���񋟂���B
*/

#pragma once

#ifdef __linux

#include "socket_base.h"

/**
*	@brief Linux�pTCP/IP�\�P�b�g�N���X
*/
class CLinuxSocket : public CSocketBase
{
	// �����o�֐�
public:
	// �R���X�g���N�^
	CLinuxSocket();

	// �\�P�b�g�ɖ��O��t����
	void wrap_bind(const uint16_t dst_port, const std::string& src_addr);
	// ���肩��̐ڑ����󂯕t����
	void wrap_listen();
	// ���肩��̐ڑ���҂�
	void wrap_accept();
	// �\�P�b�g��ڑ�����
	void wrap_connect(const std::string& addr, const uint16_t port) const;
	// �f�[�^�𑗐M����
	void wrap_send(const std::string& data, const char terminal);
	// �f�[�^����M����
	std::string wrap_recv(const char terminal);
	// �\�P�b�g�����
	void wrap_close();
	// �\�P�b�g���쐬����
	int wrap_socket();

	// �������������s��
	bool initialize();
	// �I���������s��
	bool finalize();
};

#endif // __linux