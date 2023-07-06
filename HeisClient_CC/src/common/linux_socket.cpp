/**
*	@file		linux_socket.cpp
*	@brief		Linux�pTCP/IP�\�P�b�g�N���X
*	@author		Ryo Konno
*	@details	Linux���ɂ�����\�P�b�g�̊e�푀���񋟂���B
*/

#ifdef __linux

#include "linux_socket.h"

#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <cstring>
#include <stdexcept>

/**
*	@def RECV_BUF_SIZE
*	@brief ��M�o�b�t�@�̃T�C�Y
*/
#define RECV_BUF_SIZE (1000)

/**
*	@brief �R���X�g���N�^
*	@details ������������socket�̌Ăяo���������ɍs��
*/
CLinuxSocket::CLinuxSocket()
{
	bool initialize_result = initialize();
	if (!initialize_result) {
		throw std::runtime_error("�\�P�b�g�̏��������������s���܂���");
	}

	m_socket = wrap_socket();
	if (m_socket < 0) {
		throw std::runtime_error("�\�P�b�g�̐����Ɏ��s���܂���");
	}
}

/**
*	@brief �\�P�b�g�ɖ��O��t����
*	@param[in] dst_port �ڑ���|�[�g�ԍ�
*	@param[in] src_addr �ڑ���IP�A�h���X
*/
void CLinuxSocket::wrap_bind(const uint16_t dst_port, const std::string& src_addr)
{
	sockaddr_in addr = { 0 };
	int ercd;

	addr.sin_family = AF_INET;
	addr.sin_port = htons(dst_port);
	ercd = inet_pton(AF_INET, src_addr.c_str(), &addr.sin_addr);
	if (ercd <= 0) {
		throw std::runtime_error("�w�肳�ꂽIP�A�h���X�͕s���ł�");
	}

	ercd = bind(m_socket, reinterpret_cast<sockaddr*>(&addr), sizeof(addr));
	if (ercd < 0) {
		throw std::runtime_error("bind�V�X�e���R�[���̌Ăяo���Ɏ��s���܂���");
	}
}

/**
*	@brief ���肩��̐ڑ����󂯕t����
*/
void CLinuxSocket::wrap_listen()
{
	// ���K�ɏ]���Alisten�̑�����(backlog)��5�ɂ��Ă���
	int ercd = listen(m_socket, 5);
	if (ercd < 0) {
		throw std::runtime_error("listen�V�X�e���R�[���̌Ăяo���Ɏ��s���܂���");
	}
}

/**
*	@brief ���肩��̐ڑ���҂�
*/
void CLinuxSocket::wrap_accept()
{
	sockaddr_in client_addr_info = { 0 };
	socklen_t addr_info_len = sizeof(sockaddr_in);

	// �ʐM�p�\�P�b�g�쐬
	int new_sck = accept(m_socket, reinterpret_cast<sockaddr*>(&client_addr_info), &addr_info_len);
	if (new_sck < 0) {
		throw std::runtime_error("accept�V�X�e���R�[���̌Ăяo���Ɏ��s���܂���");
	}

	// �N���C�A���g�Ƃ�1��1�Őڑ����邽�߁A�ȍ~�̐ڑ��󂯕t���͕s�v
	// ���̂��߁A���X�ڑ��󂯕t���p�������\�P�b�g�͂����ŕ���
	wrap_close();

	// �ȍ~�Am_socket�͒ʐM�p�\�P�b�g�ɂȂ�
	m_socket = new_sck;
}

/**
*	@brief �\�P�b�g��ڑ�����
*	@param[in] addr �ڑ���IP�A�h���X
*	@param[in] port �ڑ���|�[�g�ԍ�
*/
void CLinuxSocket::wrap_connect(const std::string& addr, const uint16_t port) const
{
	sockaddr_in sa = { 0 };
	int ercd;

	sa.sin_family = AF_INET;
	sa.sin_port = htons(port);
	ercd = inet_pton(AF_INET, addr.c_str(), &sa.sin_addr);
	if (ercd <= 0) {
		throw std::runtime_error("inet_pton�V�X�e���R�[���̌Ăяo���Ɏ��s���܂���");
	}

	ercd = connect(m_socket, reinterpret_cast<sockaddr*>(&sa), sizeof(sa));
	if (ercd < 0) {
		throw std::runtime_error("connect�V�X�e���R�[���̌Ăяo���Ɏ��s���܂���");
	}
}

/**
*	@brief �f�[�^�𑗐M����
*	@param[in] data ���M����f�[�^
*	@param[in] terminal �I�[����(�ȗ����͕t�^���Ȃ�)
*/
void CLinuxSocket::wrap_send(const std::string& data, const char terminal)
{
	std::string send_data = data + terminal;
	size_t sent_size = send(m_socket, send_data.c_str(), send_data.size(), 0);

	if (sent_size < send_data.size()) {
		if (sent_size < 0) {
			throw std::runtime_error("send�V�X�e���R�[���̌Ăяo���Ɏ��s���܂���");
		}
		fprintf(stderr, "�x��: �s���S�ȃf�[�^�����M����܂���(%zu�o�C�g��%zu�o�C�g�����M����܂���)\n", send_data.size(), sent_size);
	}
}

/**
*	@brief �f�[�^����M����
*	@param[in] terminal �I�[����(�ȗ����͕t�^���Ȃ�)
*/
std::string CLinuxSocket::wrap_recv(const char terminal)
{
	std::string recv_data = "";

	while (true) {
		// �\�P�b�g�̓����o�b�t�@��PEEK����
		char peek_buf[RECV_BUF_SIZE] = { 0 };
		recv(m_socket, peek_buf, sizeof(peek_buf) - 1, MSG_PEEK);

		// PEEK�����o�b�t�@����I�[������T��
		int terminal_idx = -1;
		for (int i = 0; i < RECV_BUF_SIZE - 1; i++) {
			if (peek_buf[i] == terminal) {
				terminal_idx = i;
				break;
			}
		}

		// �I�[����������΁A���̈ʒu�܂Ńf�[�^����M���ď����I��
		// ������΁Abuf�̍ő�T�C�Y�������f�[�^����M���A�ēxPEEK + ��M����
		char buf[RECV_BUF_SIZE] = { 0 };
		if (terminal_idx != -1) {
			recv(m_socket, buf, terminal_idx + 1, 0);
			recv_data += buf;
			break;
		}
		else {
			recv(m_socket, buf, sizeof(buf) - 1, 0);
			recv_data += buf;
		}
	}

	return recv_data;
}

/**
*	@brief �������������s��
*	@return bool ����������������������
*/
bool CLinuxSocket::initialize()
{
	// Do Nothing
	return true;
}

/**
*	@brief �I���������s��
*	@return bool �I������������������
*/
bool CLinuxSocket::finalize()
{
	// Do Nothing
	return true;
}

#endif // __linux