/**
*	@file		socket_base.h
*	@brief		TCP/IP�\�P�b�g�x�[�X�N���X
*	@author		Ryo Konno
*	@details	�e��v���b�g�t�H�[���ɂ�����\�P�b�g�N���X�̃x�[�X�N���X�B
*/

#pragma once

#include <string>

/**
*	@brief TCP/IP�\�P�b�g�x�[�X�N���X
*/
class CSocketBase
{
	// �����o�ϐ�
public:
	// ���[�U�R�[�h�ŌĂ΂��֐���CSocket�N���X�Œ�`����

	// �W���̃\�P�b�gAPI�Ɗ֐������Փ˂��Ȃ��悤�A�\�P�b�gAPI�̊֐����Ƃ͕ʂ̊֐����ɂ���
	// �\�P�b�gAPI�ł͖��O��Ԃ���`����Ă��Ȃ��\�������邽�߁A���O�Փ˂͊֐����̕ύX�ŉ������K�v������

	// �\�P�b�g�ɖ��O��t����
	virtual void wrap_bind(const uint16_t dst_port, const std::string& src_addr) = 0;
	// ���肩��̐ڑ����󂯕t����
	virtual void wrap_listen() = 0;
	// ���肩��̐ڑ���҂�
	virtual void wrap_accept() = 0;
	// �\�P�b�g��ڑ�����
	virtual void wrap_connect(const std::string& addr, const uint16_t port) const = 0;
	// �f�[�^�𑗐M����
	virtual void wrap_send(const std::string& data, const char terminal) = 0;
	// �f�[�^����M����
	virtual std::string wrap_recv(const char terminal) = 0;
	// �\�P�b�g�����
	virtual void wrap_close() = 0;
	// �\�P�b�g���쐬����
	virtual int wrap_socket() = 0;

	// �������������s��
	virtual bool initialize() = 0;
	// �I���������s��
	virtual bool finalize() = 0;

	// �����o�ϐ�
protected:
	//! �\�P�b�g�̎���
	int m_socket;
};