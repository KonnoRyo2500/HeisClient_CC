/**
*	@file		action_packet_builder.h
*	@brief		heis �u�s���v�p�P�b�g�쐬�N���X
*	@author		Ryo Konno
*	@details	�Ֆʏ�̕��m����u�s���v�p�P�b�g���쐬����B
*/

#pragma once

#include "JSON_data_packet.h"
#include "board.h"

/**
*	@brief �u�s���v�p�P�b�g�쐬�N���X
*/
class CActionPacketBuilder
{
	// �����o�֐�
public:
	// �u�s���v�p�P�b�g���쐬����
	JSONSendPacket_Action build(CBoard board, std::string team_name);

	// ���m���U���������W��ǉ�����
	void add_attack_destination(CInfantry infantry, BoardPosition atk_dst);

	// ������Ԃ�����������
	void clear();

private:
	// �������m���U���ς݂Ȃ�΁A�U����̍��W��Ԃ�
	BoardPosition find_attack_destination_if_attacked(std::string id);

	// �����o�ϐ�
private:
	//! �U����̍��W(ID�ƍ��W�̃y�A)
	std::vector<std::pair<std::string, BoardPosition>> m_attack_destinations;
};