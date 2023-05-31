/**
*	@file		board_json_converter.h
*	@brief		heis �u�ՖʁvJSON�ϊ��N���X
*	@author		Ryo Konno
*	@details	�u�ՖʁvJSON�̕ϊ����s���N���X�B
*/

#pragma once

#include "json_converter_base.h"
#include "JSON_data_packet.h"

/**
*	@brief	�u�ՖʁvJSON�ϊ��N���X
*/
class BoardJsonConverter : public JsonConverterBase<JSONRecvPacket_Board>
{
	// �����o�֐�
public:
	// JSON�����񂩂�p�P�b�g�ɕϊ�����
	JSONRecvPacket_Board from_json_to_packet(std::string json) const;
	// �p�P�b�g����JSON������ɕϊ�����
	std::string from_packet_to_json(JSONRecvPacket_Board pkt) const;
};