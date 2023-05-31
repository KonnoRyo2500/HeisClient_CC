/**
*	@file		name_json_converter.cpp
*	@brief		heis 「名前」JSON変換クラス
*	@author		Ryo Konno
*	@details	「名前」JSONの変換を行うクラス。
*/

#include "name_json_converter.h"

/**
*	@brief 「名前」JSONから「名前」パケットに変換する
*	@param[in] json 変換対象の「名前」JSON
*	@return std::string 変換結果の「名前」パケット
*/
JSONSendPacket_Name NameJsonConverter::from_json_to_packet(std::string json) const
{
	// クライアント側で「名前」JSONから「名前」パケットに変換する必要がないため、Do Nothing
	// 変換する必要が生じたら適宜実装する
	return JSONSendPacket_Name();
}

/**
*	@brief 「名前」パケットから「名前」JSONに変換する
*	@param[in] pkt 変換対象の「名前」パケット
*	@return std::string 変換結果の「名前」JSON
*/
std::string NameJsonConverter::from_packet_to_json(JSONSendPacket_Name pkt) const
{
	picojson::object name_obj;

	// パケットから直接変換できる要素をJSONに追加
	name_obj.insert(std::make_pair("team_name", pkt.team_name.get_value()));

	return picojson::value(name_obj).serialize();
}