/**
*	@file		game_local.h
*	@brief		heis �Q�[��(���[�J�����[�h)�i�s�Ǘ��N���X
*	@author		Ryo Konno
*	@details	���[�J�����[�h��heis�̃Q�[�������s����D
*/
#pragma once

#include "game.h"
#include "commander.h"
#include "ai_base.h"
#include "log.h"
#include "setting.h"

#include <string>

/**
*	@brief	���[�J�����[�h���s�N���X
*/
class CGameLocal : public CGame {
	// �����o�֐�
public:
		void play_game() override;

private:
		// �ŏ��̃^�[���́u�Ֆʁv�p�P�b�g���쐬����
		JSONRecvPacket_Board create_initial_board_packet(const LocalSetting& setting) const;
		// �ŏ��́u�Ֆʁv�p�P�b�g��"units"�v�f���쐬����
		std::vector<UnitsArrayElem> create_units_of_initial_board_packet(const LocalSetting& setting) const;

		// �Ֆʏ�̕��m�̍s���񐔂����Z�b�g����
		void reset_infantry_action_remain(CBoard& board) const;

		// ���̃^�[���̃`�[�������擾����
		std::string get_next_turn_team_name(const JSONRecvPacket_Board& pkt, const LocalSetting& setting) const;
		// �������Ă���`�[�������擾����
		std::string get_winning_team_name(const CBoard& board) const;
};