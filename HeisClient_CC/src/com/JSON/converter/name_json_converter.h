/**
*	@file		name_json_converter.h
*	@brief		heis 「名前」JSON変換クラス
*	@author		Ryo Konno
*	@details	「名前」JSONの変換を行うクラス。
*/

#pragma once

#include "json_converter_base.h"
#include "JSON_data_packet.h"

/**
*	@brief	「名前」JSON変換クラス
*/
class NameJsonConverter : JsonConverterBase<JSONSendPacket_Name>
{
	// メンバ関数
public:
	// JSON文字列からパケットに変換する
	JSONSendPacket_Name from_json_to_packet(const std::string& json) const;
	// パケットからJSON文字列に変換する
	std::string from_packet_to_json(const JSONSendPacket_Name& pkt) const;
};