/**
*	@file		action_json_converter.h
*	@brief		heis �u�s���vJSON�ϊ��N���X
*	@author		Ryo Konno
*	@details	�u�s���vJSON�̕ϊ����s���N���X�B
*/

#pragma once

#include "json_converter_base.h"
#include "JSON_data_packet.h"

/**
*	@brief	�u�s���vJSON�ϊ��N���X
*/
class ActionJsonConverter : public JsonConverterBase<JSONSendPacket_Action>
{
	// �����o�֐�
public:
	// JSON�����񂩂�p�P�b�g�ɕϊ�����
	JSONSendPacket_Action from_json_to_packet(const std::string& json) const;
	// �p�P�b�g����JSON������ɕϊ�����
	std::string from_packet_to_json(const JSONSendPacket_Action& pkt) const;
};