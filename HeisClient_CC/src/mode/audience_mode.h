/**
*	@file		audience_mode.h
*	@brief		heis �ϐ탂�[�h�N���X
*	@author		Ryo Konno
*	@details	heis�̑ΐ���ϐ킷��D
*/
#pragma once

#include "game.h"
#include "socket.h"
#include "log.h"
#include "setting.h"

/**
*	@brief	�ϐ탂�[�h�N���X
*	@remark ���̃��[�h�Ɠ��l�ɃC���X�^���X����舵���������߁CCGame�N���X�̔h���N���X�Ƃ���
*/
class CAudienceMode : public CGame {
	// �\���́C�񋓑̂Ȃ�
	private:

	// �����o�֐�
	public:
		// �ϐ탁�C������
		void play_game() override;
};