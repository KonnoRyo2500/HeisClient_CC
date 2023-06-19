/**
*	@file		confirm_name_json_converter.cpp
*	@brief		heis �u���O�m��vJSON�ϊ��N���X
*	@author		Ryo Konno
*	@details	�u���O�m��vJSON�̕ϊ����s���N���X�B
*/

#include "confirm_name_json_converter.h"

/**
*	@brief �u���O�m��vJSON����u���O�m��v�p�P�b�g�ɕϊ�����
*	@param[in] json �ϊ��Ώۂ́u���O�m��vJSON
*	@return std::string �ϊ����ʂ́u���O�m��v�p�P�b�g
*/
JSONRecvPacket_ConfirmName ConfirmNameJsonConverter::from_json_to_packet(const std::string& json) const
{
	JSONRecvPacket_ConfirmName confirm_name_pkt;
	picojson::object root_obj = parse_json(json);

	// ���[�g�I�u�W�F�N�g���璼�ڎ擾�ł���l���擾
	confirm_name_pkt.your_team.set_value(root_obj["your_team"].get<std::string>());

	return confirm_name_pkt;
}

/**
*	@brief �u���O�m��v�p�P�b�g����u���O�m��vJSON�ɕϊ�����
*	@param[in] pkt �ϊ��Ώۂ́u���O�m��v�p�P�b�g
*	@return std::string �ϊ����ʂ́u���O�m��vJSON
*/
std::string ConfirmNameJsonConverter::from_packet_to_json(const JSONRecvPacket_ConfirmName& pkt) const
{
	// �N���C�A���g���Łu���O�m��v�p�P�b�g����u���O�m��vJSON�ɕϊ�����K�v���Ȃ����߁ADo Nothing
	// �ϊ�����K�v����������K�X��������
	return "";
}