/**
*	@file		socket.cpp
*	@brief		TCP/IP�\�P�b�g�N���X
*	@author		Ryo Konno
*	@details	�v���b�g�t�H�[���Ɉ˂�Ȃ�TCP/IP�\�P�b�g�����񋟂���B
*/

#include "socket.h"
#include "windows_socket.h"
#include "linux_socket.h"

#include <stdexcept>
#include <assert.h>

/**
*	@brief �R���X�g���N�^
*	@details socket�������s����
*	@throws runtime_error OS���T�|�[�g����Ă��Ȃ��ꍇ
*/
CSocket::CSocket()
{
	// �v���b�g�t�H�[���ɉ����āA�\�P�b�g�N���X�̃C���X�^���X���쐬����
#if defined(WIN32)
	m_socket = new CWindowsSocket();
#elif defined(__linux)
	m_socket = new CLinuxSocket();
#else
	m_socket = NULL;
#endif

	if (m_socket == NULL) {
		throw std::runtime_error("�\�P�b�g�N���X�͂��g����OS���T�|�[�g���Ă��܂���");
	}
}

/**
*	@brief �\�P�b�g�ɖ��O��t����
*	@param[in] dst_port �ڑ���|�[�g�ԍ�
*	@param[in] src_addr �ڑ���IP�A�h���X
*/
void CSocket::bind(const uint16_t dst_port, const std::string& src_addr)
{
	assert(m_socket != NULL);
	m_socket->wrap_bind(dst_port, src_addr);
}

/**
*	@brief ���肩��̐ڑ����󂯕t����
*/
void CSocket::listen()
{
	assert(m_socket != NULL);
	m_socket->wrap_listen();
}

/**
*	@brief ���肩��̐ڑ���҂�
*/
void CSocket::accept()
{
	assert(m_socket != NULL);
	m_socket->wrap_accept();
}

/**
*	@brief �\�P�b�g��ڑ�����
*	@param[in] addr �ڑ���IP�A�h���X
*	@param[in] port �ڑ���|�[�g�ԍ�
*/
void CSocket::connect(const std::string& addr, const uint16_t port) const
{
	assert(m_socket != NULL);
	m_socket->wrap_connect(addr, port);
}

/**
*	@brief �f�[�^�𑗐M����
*	@param[in] data ���M����f�[�^
*	@param[in] terminal �I�[����(�ȗ����͕t�^���Ȃ�)
*/
void CSocket::send(const std::string& data, const char terminal)
{
	assert(m_socket != NULL);
	m_socket->wrap_send(data, terminal);
}

/**
*	@brief �f�[�^����M����
*	@param[in] terminal �I�[����(�ȗ����͕t�^���Ȃ�)
*/
std::string CSocket::recv(const char terminal)
{
	assert(m_socket != NULL);
	return m_socket->wrap_recv(terminal);
}

/**
*	@brief �\�P�b�g�����
*/
void CSocket::close()
{
	assert(m_socket != NULL);
	m_socket->wrap_close();

	// �\�P�b�g�̏I��������j���������ōs��
	// �\�P�b�g�̏I������
	bool finalize_result = m_socket->finalize();
	if (!finalize_result) {
		throw std::runtime_error("�\�P�b�g�̏I�������Ɏ��s���܂���");
	}

	// �\�P�b�g�̔j��
	delete m_socket;
	m_socket = NULL;
}