/**
*	@file		message_json_converter.cpp
*	@brief		heis 「メッセージ」JSON変換クラス
*	@author		Ryo Konno
*	@details	「メッセージ」JSONの変換を行うクラス。
*/

#include "message_json_converter.h"

/**
*	@brief 「メッセージ」JSONから「メッセージ」パケットに変換する
*	@param[in] json 変換対象の「メッセージ」JSON
*	@return std::string 変換結果の「メッセージ」パケット
*/
JSONRecvPacket_Message MessageJsonConverter::from_json_to_packet(std::string json) const
{
	JSONRecvPacket_Message message_pkt;
	picojson::object root_obj = parse_json(json);

	// ルートオブジェクトから直接取得できる値を取得
	message_pkt.message.set_value(root_obj["message"].get<std::string>());

	return message_pkt;
}

/**
*	@brief 「メッセージ」パケットから「メッセージ」JSONに変換する
*	@param[in] pkt 変換対象の「メッセージ」パケット
*	@return std::string 変換結果の「メッセージ」JSON
*/
std::string MessageJsonConverter::from_packet_to_json(JSONRecvPacket_Message pkt) const
{
	// クライアント側で「メッセージ」パケットから「メッセージ」JSONに変換する必要がないため、Do Nothing
	// 変換する必要が生じたら適宜実装する
	return "";
}