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
	JSONSendPacket_Action build(const CBoard& board, const std::string& team_name) const;

	// ���m���U���������W��ǉ�����
	void add_attack_destination(const CInfantry& infantry, const Coordinate2D& atk_dst);

	// ������Ԃ�����������
	void clear();

private:
	// �������m���U���ς݂Ȃ�΁A�U����̍��W��Ԃ�
	Coordinate2D find_attack_destination_if_attacked(const std::string& id) const;

	// �����o�ϐ�
private:
	//! �U����̍��W(ID�ƍ��W�̃y�A)
	std::vector<std::pair<std::string, Coordinate2D>> m_attack_destinations;
};