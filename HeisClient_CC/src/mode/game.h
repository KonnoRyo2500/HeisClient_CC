/**
*	@file		game.h
*	@brief		heis �Q�[���x�[�X�N���X
*	@author		Ryo Konno
*	@details	�e��Q�[�����[�h�̃C���^�[�t�F�C�X���`����B
*/
#pragma once

/**
*	@brief �Q�[����{�N���X
*/
class CGame {
	// �����o�֐�
	public:
		// �R���X�g���N�^
		CGame();
		// �f�X�g���N�^
		virtual ~CGame();

		// �ΐ�̎��s
		virtual void play_game() = 0;
};