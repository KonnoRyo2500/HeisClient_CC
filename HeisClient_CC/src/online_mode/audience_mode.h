/**
*	@file		audience_mode.h
*	@brief		heis �ϐ탂�[�h�N���X
*	@author		Ryo Konno
*	@details	heis�̑ΐ���ϐ킷��D
*/
#pragma once

#include "client_socket.h"
#include "log.h"

extern CLog g_system_log;

/**
*	@brief	�ϐ탂�[�h�N���X
*/
class CAudienceMode {
	// �\���́C�񋓑̂Ȃ�
	private:

	// �����o�֐�
	public:
		void watch_game();

	private:
		// �ϐ�̏�����
		void initialize_watch();

		// �ϐ�̏I������
		void finalize_watch();

	// �����o�ϐ�
	private:
		//! TCP�\�P�b�g
		CClientSocket *m_sck;
};