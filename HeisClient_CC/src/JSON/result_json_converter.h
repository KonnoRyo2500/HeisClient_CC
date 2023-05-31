/**
*	@file		result_json_converter.h
*	@brief		heis �u���ʁvJSON�ϊ��N���X
*	@author		Ryo Konno
*	@details	�u���ʁvJSON�̕ϊ����s���N���X�B
*/

#pragma once

#include "json_converter_base.h"
#include "JSON_data_packet.h"

/**
*	@brief	�u���ʁvJSON�ϊ��N���X
*/
class ResultJsonConverter : JsonConverterBase<JSONRecvPacket_Result>
{
	// �����o�֐�
public:
	// JSON�����񂩂�p�P�b�g�ɕϊ�����
	JSONRecvPacket_Result from_json_to_packet(std::string json) const;
	// �p�P�b�g����JSON������ɕϊ�����
	std::string from_packet_to_json(JSONRecvPacket_Result pkt) const;
};