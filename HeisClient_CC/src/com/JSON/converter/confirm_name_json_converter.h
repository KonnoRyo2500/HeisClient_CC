/**
*	@file		confirm_name_json_converter.h
*	@brief		heis �u���O�m��vJSON�ϊ��N���X
*	@author		Ryo Konno
*	@details	�u���O�m��vJSON�̕ϊ����s���N���X�B
*/

#pragma once

#include "json_converter_base.h"
#include "JSON_data_packet.h"

/**
*	@brief	�u���O�m��vJSON�ϊ��N���X
*/
class ConfirmNameJsonConverter : JsonConverterBase<JSONRecvPacket_ConfirmName>
{
	// �����o�֐�
public:
	// JSON�����񂩂�p�P�b�g�ɕϊ�����
	JSONRecvPacket_ConfirmName from_json_to_packet(const std::string& json) const;
	// �p�P�b�g����JSON������ɕϊ�����
	std::string from_packet_to_json(const JSONRecvPacket_ConfirmName& pkt) const;
};