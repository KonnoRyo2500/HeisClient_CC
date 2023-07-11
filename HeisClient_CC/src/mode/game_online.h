/**
*	@file		game_online.h
*	@brief		heis �Q�[��(�I�����C�����[�h)�i�s�Ǘ��N���X
*	@author		Ryo Konno
*	@details	�I�����C�����[�h��heis�̃Q�[�������s����D
*/
#pragma once

#include "game.h"
#include "commander.h"
#include "ai_base.h"
#include "socket.h"
#include "setting.h"
#include <string>
#include <vector>

/**
*	@brief	�I�����C�����[�h���s�N���X
*/
class CGameOnline : public CGame {
	// �����o�֐�
	public:
		// �ΐ�J�n
		void play_game() override;
		
	private:
		// �ΐ�J�n�O����
		void initialize_battle(const OnlineSetting& setting);
		void recv_name_request() const;
		void name_entry(const std::string& name) const;
		void name_register(const OnlineSetting& setting);

		// �ΐ�I���㏈��
		void finalize_battle();

		// �ΐ��̏��s����
		bool judge_win() const;

	// �����o�ϐ�
	private:
		//! ���`�[����
		std::string m_team_name;

		//! �i�ߊ�
		CCommander* m_commander;

		//! AI
		CAIBase* m_ai;

		//! TCP�\�P�b�g
		CSocket* m_sck;
};