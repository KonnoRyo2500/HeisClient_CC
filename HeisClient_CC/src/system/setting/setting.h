/**
*	@file		setting.h
*	@brief		heis �ݒ�t�@�C���\����
*	@author		Ryo Konno
*	@details	�e��Q�[�����[�h�ł̐ݒ�l���i�[���邽�߂̍\���̂��`����B
*/

#pragma once

#include <string>
#include <vector>

#include "coodinate_2d.h"

/**
*	@struct LocalSetting
*	@brief ���[�J�����[�h�ݒ�l�i�[�p�\����
*/
struct LocalSetting {
	//! ���`�[����
	std::string my_team_name;
	//! �G�`�[����
	std::string enemy_team_name;
	//! ���`�[����AI����
	std::string my_team_ai_impl;
	//! �G�`�[����AI����
	std::string enemy_team_ai_impl;
	//! �Ֆʂ̉���
	uint16_t board_width;
	//! �Ֆʂ̍���
	uint16_t board_height;
	//! ���`�[�����m�̏����ʒu
	std::vector<Coordinate2D> my_team_init_pos;
	//! �G�`�[�����m�̏����ʒu
	std::vector<Coordinate2D> enemy_team_init_pos;
	//! ��U�ƂȂ�`�[����
	std::string first_turn_team;
};

/**
*	@struct OnlineSetting
*	@brief �I�����C�����[�h�ݒ�l�i�[�p�\����
*/
struct OnlineSetting {
	//! �T�[�o��IP�A�h���X
	std::string server_ip_addr;
	//! �T�[�o�̃|�[�g�ԍ�
	uint16_t server_port_num;
	//! �`�[����
	std::string team_name;
	//! AI����
	std::string ai_impl;
};

/**
*	@struct AudienceSetting
*	@brief �ϐ탂�[�h�ݒ�l�i�[�p�\����
*/
struct AudienceSetting {
	//! �T�[�o��IP�A�h���X
	std::string server_ip_addr;
	//! �T�[�o�̃|�[�g�ԍ�
	uint16_t server_port_num;
};