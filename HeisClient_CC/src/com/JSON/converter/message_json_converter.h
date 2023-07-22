/**
*	@file		message_json_converter.h
*	@brief		heis �u���b�Z�[�W�vJSON�ϊ��N���X
*	@author		Ryo Konno
*	@details	�u���b�Z�[�W�vJSON�̕ϊ����s���N���X�B
*/

#pragma once

#include "json_converter_base.h"
#include "JSON_data_packet.h"

/**
*	@brief	�u���b�Z�[�W�vJSON�ϊ��N���X
*/
class MessageJsonConverter : JsonConverterBase<JSONRecvPacket_Message>
{
	// �����o�֐�
public:
	// JSON�����񂩂�p�P�b�g�ɕϊ�����
	JSONRecvPacket_Message from_json_to_packet(const std::string& json) const;
	// �p�P�b�g����JSON������ɕϊ�����
	std::string from_packet_to_json(const JSONRecvPacket_Message& pkt) const;
};