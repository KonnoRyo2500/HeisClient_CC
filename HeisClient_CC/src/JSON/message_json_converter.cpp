/**
*	@file		message_json_converter.cpp
*	@brief		heis �u���b�Z�[�W�vJSON�ϊ��N���X
*	@author		Ryo Konno
*	@details	�u���b�Z�[�W�vJSON�̕ϊ����s���N���X�B
*/

#include "message_json_converter.h"

/**
*	@brief �u���b�Z�[�W�vJSON����u���b�Z�[�W�v�p�P�b�g�ɕϊ�����
*	@param[in] json �ϊ��Ώۂ́u���b�Z�[�W�vJSON
*	@return std::string �ϊ����ʂ́u���b�Z�[�W�v�p�P�b�g
*/
JSONRecvPacket_Message MessageJsonConverter::from_json_to_packet(std::string json) const
{
	JSONRecvPacket_Message message_pkt;
	picojson::object root_obj = parse_json(json);

	// ���[�g�I�u�W�F�N�g���璼�ڎ擾�ł���l���擾
	message_pkt.message.set_value(root_obj["message"].get<std::string>());

	return message_pkt;
}

/**
*	@brief �u���b�Z�[�W�v�p�P�b�g����u���b�Z�[�W�vJSON�ɕϊ�����
*	@param[in] pkt �ϊ��Ώۂ́u���b�Z�[�W�v�p�P�b�g
*	@return std::string �ϊ����ʂ́u���b�Z�[�W�vJSON
*/
std::string MessageJsonConverter::from_packet_to_json(JSONRecvPacket_Message pkt) const
{
	// �N���C�A���g���Łu���b�Z�[�W�v�p�P�b�g����u���b�Z�[�W�vJSON�ɕϊ�����K�v���Ȃ����߁ADo Nothing
	// �ϊ�����K�v����������K�X��������
	return "";
}