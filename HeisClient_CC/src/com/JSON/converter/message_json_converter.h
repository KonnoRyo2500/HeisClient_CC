/**
*	@file		message_json_converter.h
*	@brief		heis 「メッセージ」JSON変換クラス
*	@author		Ryo Konno
*	@details	「メッセージ」JSONの変換を行うクラス。
*/

#pragma once

#include "json_converter_base.h"
#include "JSON_data_packet.h"

/**
*	@brief	「メッセージ」JSON変換クラス
*/
class MessageJsonConverter : JsonConverterBase<JSONRecvPacket_Message>
{
	// メンバ関数
public:
	// JSON文字列からパケットに変換する
	JSONRecvPacket_Message from_json_to_packet(const std::string& json) const;
	// パケットからJSON文字列に変換する
	std::string from_packet_to_json(const JSONRecvPacket_Message& pkt) const;
};