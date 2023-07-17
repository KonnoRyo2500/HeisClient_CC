/**
*	@file		commander.h
*	@brief		heis �i�ߊ��N���X
*	@author		Ryo Konno
*	@details	�`�[���̕��m���ꌳ�Ǘ����A������Ԏ擾���s���B
*/
#pragma once

#include "infantry.h"
#include "board_observer.h"
#include "board_controller.h"
#include "action_packet_builder.h"

#include <vector>

/**
*	@brief	�i�ߊ��N���X
*	@details AI�́A���̃N���X�̊e�탁���o�֐����Ăяo�����ƂŁA�Ֆʏ�̕��m�𑀍�ł���B
*/
class CCommander
{
	// �����o�֐�
public:
	// �R���X�g���N�^
	explicit CCommander(const std::string& team_name, CBoard* board);

	// �w�肵�����m�̈ʒu���擾����
	Coordinate2D get_infantry_position_by_id(const std::string& id) const;
	// �w�肵�����m�̃X�e�[�^�X���擾����
	InfantryStatus get_infantry_status_by_id(const std::string& id) const;

	// ���m�ɍU�����w������
	void attack(const std::string& id, const Coordinate2D& dst);
	// ���m�Ɉړ����w������
	void move(const std::string& id, const Coordinate2D& dst) const;

	// �ړ��\�Ȃ��ׂẴ}�X���擾����
	std::vector<Coordinate2D> find_movable_position(const std::string& id) const;
	// �U���\�Ȃ��ׂẴ}�X���擾����
	std::vector<Coordinate2D> find_attackable_position(const std::string& id) const;

	// �s���\�ȕ��m��ID���擾����
	std::vector<std::string> get_all_actable_infantry_ids(const std::string& team_name) const;
	// �ړ��\�ȕ��m��ID���擾����
	std::vector<std::string> get_all_movable_infantry_ids(const std::string& team_name) const;
	// �U���\�ȕ��m��ID���擾����
	std::vector<std::string> get_all_attackable_infantry_ids(const std::string& team_name) const;

	// �u�s���v�p�P�b�g���쐬����
	JSONSendPacket_Action create_action_pkt();

private:
	// ID���畺�m���擾����
	InfantryWithPos find_infantry_by_id(const std::string& id) const;

	// �����o�ϐ�
private:
	//! ���`�[����
	std::string m_team_name;

	//! �Ֆ�
	CBoard* m_board;

	//! �Ֆʏ��擾�N���X
	CBoardObserver m_observer;
	//! �Ֆʑ���N���X
	CBoardController m_controller;
	//! �u�s���v�p�P�b�g�쐬�N���X
	CActionPacketBuilder m_builder;
};