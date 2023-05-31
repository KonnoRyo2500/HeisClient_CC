/**
*	@file		name_json_converter.cpp
*	@brief		heis �u���O�vJSON�ϊ��N���X
*	@author		Ryo Konno
*	@details	�u���O�vJSON�̕ϊ����s���N���X�B
*/

#include "name_json_converter.h"

/**
*	@brief �u���O�vJSON����u���O�v�p�P�b�g�ɕϊ�����
*	@param[in] json �ϊ��Ώۂ́u���O�vJSON
*	@return std::string �ϊ����ʂ́u���O�v�p�P�b�g
*/
JSONSendPacket_Name NameJsonConverter::from_json_to_packet(std::string json) const
{
	// �N���C�A���g���Łu���O�vJSON����u���O�v�p�P�b�g�ɕϊ�����K�v���Ȃ����߁ADo Nothing
	// �ϊ�����K�v����������K�X��������
	return JSONSendPacket_Name();
}

/**
*	@brief �u���O�v�p�P�b�g����u���O�vJSON�ɕϊ�����
*	@param[in] pkt �ϊ��Ώۂ́u���O�v�p�P�b�g
*	@return std::string �ϊ����ʂ́u���O�vJSON
*/
std::string NameJsonConverter::from_packet_to_json(JSONSendPacket_Name pkt) const
{
	picojson::object name_obj;

	// �p�P�b�g���璼�ڕϊ��ł���v�f��JSON�ɒǉ�
	name_obj.insert(std::make_pair("team_name", pkt.team_name.get_value()));

	return picojson::value(name_obj).serialize();
}