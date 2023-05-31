/**
*	@file		result_json_converter.cpp
*	@brief		heis �u���ʁvJSON�ϊ��N���X
*	@author		Ryo Konno
*	@details	�u���ʁvJSON�̕ϊ����s���N���X�B
*/

#include "result_json_converter.h"

/**
*	@brief �u���ʁvJSON����u���ʁv�p�P�b�g�ɕϊ�����
*	@param[in] json �ϊ��Ώۂ́u���ʁvJSON
*	@return std::string �ϊ����ʂ́u���ʁv�p�P�b�g
*/
JSONRecvPacket_Result ResultJsonConverter::from_json_to_packet(std::string json) const
{
	JSONRecvPacket_Result result_pkt;
	picojson::object root_obj = parse_json(json);

	// "result"�z��̒l���擾
	std::vector<ResultArrayElem> result;
	picojson::array result_array = root_obj["result"].get<picojson::array>();
	for (auto& val : result_array) {
		ResultArrayElem elem;
		picojson::object obj = val.get<picojson::object>();

		// "unit_id"�͏ȗ����ꂤ�邽�߁AJSON�ɃL�[�����݂��邩�ǂ����𔻒肵�Ă���擾����
		if (obj.find("unit_id") != obj.end()) {
			elem.unit_id.set_value(obj["unit_id"].get<std::string>());
		}
		elem.error.set_value(obj["error"].get<std::string>());

		result.push_back(elem);
	}
	result_pkt.result.set_value(result);

	return result_pkt;
}

/**
*	@brief �u���ʁv�p�P�b�g����u���ʁvJSON�ɕϊ�����
*	@param[in] pkt �ϊ��Ώۂ́u���ʁv�p�P�b�g
*	@return std::string �ϊ����ʂ́u���ʁvJSON
*/
std::string ResultJsonConverter::from_packet_to_json(JSONRecvPacket_Result pkt) const
{
	// �N���C�A���g���Łu���ʁv�p�P�b�g����u���ʁvJSON�ɕϊ�����K�v���Ȃ����߁ADo Nothing
	// �ϊ�����K�v����������K�X��������
	return "";
}