/**
*	@file		action_json_converter.cpp
*	@brief		heis �u�s���vJSON�ϊ��N���X
*	@author		Ryo Konno
*	@details	�u�s���vJSON�̕ϊ����s���N���X�B
*/

#include "action_json_converter.h"

/**
*	@brief �u�s���vJSON����u�s���v�p�P�b�g�ɕϊ�����
*	@param[in] json �ϊ��Ώۂ́u�s���vJSON
*	@return std::string �ϊ����ʂ́u�s���v�p�P�b�g
*/
JSONSendPacket_Action ActionJsonConverter::from_json_to_packet(const std::string& json) const
{
	// �N���C�A���g���Łu�s���vJSON����u�s���v�p�P�b�g�ɕϊ�����K�v���Ȃ����߁ADo Nothing
	// �ϊ�����K�v����������K�X��������
	return JSONSendPacket_Action();
}

/**
*	@brief �u�Ֆʁv�p�P�b�g����u�ՖʁvJSON�ɕϊ�����
*	@param[in] pkt �ϊ��Ώۂ́u�Ֆʁv�p�P�b�g
*	@return std::string �ϊ����ʂ́u�ՖʁvJSON
*/
std::string ActionJsonConverter::from_packet_to_json(const JSONSendPacket_Action& pkt) const
{
	picojson::object action_obj;

	// �p�P�b�g���璼�ڕϊ��ł���v�f��JSON�ɒǉ�
	action_obj.insert(std::make_pair("turn_team", pkt.turn_team.get_value()));

	// "contents"�z����쐬
	picojson::array contents;
	for (auto& content : pkt.contents.get_value()) {
		picojson::object contents_obj;

		// "to"�I�u�W�F�N�g�̍쐬
		{
			picojson::object to_obj;

			to_obj.insert(std::make_pair("x", picojson::value(static_cast<double>(content.to_x.get_value()))));
			to_obj.insert(std::make_pair("y", picojson::value(static_cast<double>(content.to_y.get_value()))));
			contents_obj.insert(std::make_pair("to", picojson::value(to_obj)));
		}

		// "atk"�I�u�W�F�N�g�̍쐬
		{
			picojson::object atk_obj;

			atk_obj.insert(std::make_pair("x", picojson::value(static_cast<double>(content.atk_x.get_value()))));
			atk_obj.insert(std::make_pair("y", picojson::value(static_cast<double>(content.atk_y.get_value()))));
			contents_obj.insert(std::make_pair("atk", picojson::value(atk_obj)));
		}

		// �쐬�����e�I�u�W�F�N�g�ƁC���j�b�gID��z��v�f�Ƃ��Ēǉ�
		contents_obj.insert(std::make_pair("unit_id", content.unit_id.get_value()));
		contents.push_back(picojson::value(contents_obj));
	}
	action_obj.insert(std::make_pair("contents", "x"));

	return picojson::value(action_obj).serialize();
}