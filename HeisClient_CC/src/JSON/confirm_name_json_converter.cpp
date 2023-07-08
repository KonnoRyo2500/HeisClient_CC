/**
*	@file		confirm_name_json_converter.cpp
*	@brief		heis 「名前確定」JSON変換クラス
*	@author		Ryo Konno
*	@details	「名前確定」JSONの変換を行うクラス。
*/

#include "confirm_name_json_converter.h"

/**
*	@brief 「名前確定」JSONから「名前確定」パケットに変換する
*	@param[in] json 変換対象の「名前確定」JSON
*	@return std::string 変換結果の「名前確定」パケット
*/
JSONRecvPacket_ConfirmName ConfirmNameJsonConverter::from_json_to_packet(const std::string& json) const
{
	JSONRecvPacket_ConfirmName confirm_name_pkt;
	picojson::object root_obj = parse_json(json);

	// ルートオブジェクトから直接取得できる値を取得
	confirm_name_pkt.your_team.set_value(root_obj["your_team"].get<std::string>());

	return confirm_name_pkt;
}

/**
*	@brief 「名前確定」パケットから「名前確定」JSONに変換する
*	@param[in] pkt 変換対象の「名前確定」パケット
*	@return std::string 変換結果の「名前確定」JSON
*/
std::string ConfirmNameJsonConverter::from_packet_to_json(const JSONRecvPacket_ConfirmName& pkt) const
{
	// クライアント側で「名前確定」パケットから「名前確定」JSONに変換する必要がないため、Do Nothing
	// 変換する必要が生じたら適宜実装する
	return "";
}