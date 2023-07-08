/**
*	@file		board_json_converter.cpp
*	@brief		heis 「盤面」JSON変換クラス
*	@author		Ryo Konno
*	@details	「盤面」JSONの変換を行うクラス。
*/

#include "board_json_converter.h"

/**
*	@brief 「盤面」JSONから「盤面」パケットに変換する
*	@param[in] json 変換対象の「盤面」JSON
*	@return std::string 変換結果の「盤面」パケット
*/
JSONRecvPacket_Board BoardJsonConverter::from_json_to_packet(const std::string& json) const
{
	JSONRecvPacket_Board board_pkt;
	picojson::object root_obj = parse_json(json);

	// ルートオブジェクトから直接取得できる値を取得
	board_pkt.width.set_value((uint16_t)root_obj["width"].get<double>());
	board_pkt.height.set_value((uint16_t)root_obj["height"].get<double>());
	board_pkt.turn_team.set_value(root_obj["turn_team"].get<std::string>());
	board_pkt.finished.set_value(root_obj["finished"].get<bool>());
	board_pkt.count.set_value((uint32_t)root_obj["count"].get<double>());

	// "players"配列の値を取得
	std::vector<std::string> players;
	picojson::array players_array = root_obj["players"].get<picojson::array>();
	for (auto& val : players_array) {
		players.push_back(val.get<std::string>());
	}
	board_pkt.players.set_value(players);

	// "units"配列の値を取得
	std::vector<UnitsArrayElem> units;
	picojson::array units_array = root_obj["units"].get<picojson::array>();
	for (auto& val : units_array) {
		UnitsArrayElem elem;
		picojson::object obj = val.get<picojson::object>();

		elem.hp.set_value((int8_t)obj["hp"].get<double>());
		elem.unit_id.set_value(obj["unit_id"].get<std::string>());
		elem.type.set_value(obj["type"].get<std::string>());
		elem.team.set_value(obj["team"].get<std::string>());

		// "locate"オブジェクトの値を取得
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
*	@brief 「盤面」パケットから「盤面」JSONに変換する
*	@param[in] pkt 変換対象の「盤面」パケット
*	@return std::string 変換結果の「盤面」JSON
*/
std::string BoardJsonConverter::from_packet_to_json(const JSONRecvPacket_Board& pkt) const
{
	// クライアント側で「盤面」パケットから「盤面」JSONに変換する必要がないため、Do Nothing
	// 変換する必要が生じたら適宜実装する
	return "";
}