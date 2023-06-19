/**
*	@file		name_json_converter.h
*	@brief		heis �u���O�vJSON�ϊ��N���X
*	@author		Ryo Konno
*	@details	�u���O�vJSON�̕ϊ����s���N���X�B
*/

#pragma once

#include "json_converter_base.h"
#include "JSON_data_packet.h"

/**
*	@brief	�u���O�vJSON�ϊ��N���X
*/
class NameJsonConverter : JsonConverterBase<JSONSendPacket_Name>
{
	// �����o�֐�
public:
	// JSON�����񂩂�p�P�b�g�ɕϊ�����
	JSONSendPacket_Name from_json_to_packet(const std::string& json) const;
	// �p�P�b�g����JSON������ɕϊ�����
	std::string from_packet_to_json(const JSONSendPacket_Name& pkt) const;
};