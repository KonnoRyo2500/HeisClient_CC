/**
*	@file		board_json_converter.cpp
*	@brief		heis �u�ՖʁvJSON�ϊ��N���X
*	@author		Ryo Konno
*	@details	�u�ՖʁvJSON�̕ϊ����s���N���X�B
*/

#include "board_json_converter.h"

/**
*	@brief �u�ՖʁvJSON����u�Ֆʁv�p�P�b�g�ɕϊ�����
*	@param[in] json �ϊ��Ώۂ́u�ՖʁvJSON
*	@return std::string �ϊ����ʂ́u�Ֆʁv�p�P�b�g
*/
JSONRecvPacket_Board BoardJsonConverter::from_json_to_packet(std::string json) const
{
	JSONRecvPacket_Board board_pkt;
	picojson::object root_obj = parse_json(json);

	// ���[�g�I�u�W�F�N�g���璼�ڎ擾�ł���l���擾
	board_pkt.width.set_value((uint16_t)root_obj["width"].get<double>());
	board_pkt.height.set_value((uint16_t)root_obj["height"].get<double>());
	board_pkt.turn_team.set_value(root_obj["turn_team"].get<std::string>());
	board_pkt.finished.set_value(root_obj["finished"].get<bool>());
	board_pkt.count.set_value((uint32_t)root_obj["count"].get<double>());

	// "players"�z��̒l���擾
	std::vector<std::string> players;
	picojson::array players_array = root_obj["players"].get<picojson::array>();
	for (auto& val : players_array) {
		players.push_back(val.get<std::string>());
	}
	board_pkt.players.set_value(players);

	// "units"�z��̒l���擾
	std::vector<UnitsArrayElem> units;
	picojson::array units_array = root_obj["units"].get<picojson::array>();
	for (auto& val : units_array) {
		UnitsArrayElem elem;
		picojson::object obj = val.get<picojson::object>();

		elem.hp.set_value((int8_t)obj["hp"].get<double>());
		elem.unit_id.set_value(obj["unit_id"].get<std::string>());
		elem.type.set_value(obj["type"].get<std::string>());
		elem.team.set_value(obj["team"].get<std::string>());

		// "locate"�I�u�W�F�N�g�̒l���擾
		LocateObjData locate;
		picojson::object locate_obj = obj["locate"].get<picojson::object>();
		locate.x.set_value((uint16_t)locate_obj["x"].get<double>());
		locate.y.set_value((uint16_t)locate_obj["y"].get<double>());
		elem.locate.set_value(locate);

		units.push_back(elem);
	}
	board_pkt.units.set_value(units);

	return board_pkt;
}

/**
*	@brief �u�Ֆʁv�p�P�b�g����u�ՖʁvJSON�ɕϊ�����
*	@param[in] pkt �ϊ��Ώۂ́u�Ֆʁv�p�P�b�g
*	@return std::string �ϊ����ʂ́u�ՖʁvJSON
*/
std::string BoardJsonConverter::from_packet_to_json(JSONRecvPacket_Board pkt) const
{
	// �N���C�A���g���Łu�Ֆʁv�p�P�b�g����u�ՖʁvJSON�ɕϊ�����K�v���Ȃ����߁ADo Nothing
	// �ϊ�����K�v����������K�X��������
	return "";
}