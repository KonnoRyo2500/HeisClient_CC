/**
*	@file		json_converter_base.h
*	@brief		heis JSON�ϊ��x�[�X�N���X
*	@author		Ryo Konno
*	@details	�e��JSON��̓N���X(JSON <-> �p�P�b�g�̑��ݕϊ����s���N���X)�̃x�[�X�N���X�B
*/

#pragma once

#include "picojson.h"
#include "string_utils.h"

/**
*	@brief	JSON��̓x�[�X�N���X
*	@tparam PacketType ��舵��JSON�p�P�b�g�̌^
*/
template <typename PacketType>
class JsonConverterBase
{
	// �����o�֐�
public:
	// JSON�����񂩂�p�P�b�g�ɕϊ�����
	virtual PacketType from_json_to_packet(const std::string& json) const = 0;
	// �p�P�b�g����JSON������ɕϊ�����
	virtual std::string from_packet_to_json(const PacketType& pkt) const = 0;

protected:
	// JSON��������p�[�X����
	picojson::object parse_json(const std::string& json) const;
};

/**
*	@brief	JSON���p�[�X����
*	@tparam PacketType ��舵��JSON�p�P�b�g�̌^
*	@param[in] json �p�[�X�Ώۂ�JSON
*	@return picojson::object �p�[�X���ꂽJSON�I�u�W�F�N�g
*/
template <typename PacketType>
picojson::object JsonConverterBase<PacketType>::parse_json(const std::string& json) const
{
	picojson::value parsed_JSON_val;
	std::string errmsg = picojson::parse(parsed_JSON_val, json);
	if (errmsg.size() > 0) {
		throw std::runtime_error(CStringUtil::format("JSON�̃p�[�X�Ɏ��s���܂���(�G���[���e: %s)", errmsg.c_str()));
	}

	// JSON -> �f�[�^�ւ̕ϊ����s���₷�����邽�߁Cpicojson::value -> picojson::object�ւ̕ϊ����s��
	return parsed_JSON_val.get<picojson::object>();
}