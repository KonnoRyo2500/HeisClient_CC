/**
*	@file		confirm_name_json_converter.h
*	@brief		heis 「名前確定」JSON変換クラス
*	@author		Ryo Konno
*	@details	「名前確定」JSONの変換を行うクラス。
*/

#pragma once

#include "json_converter_base.h"
#include "JSON_data_packet.h"

/**
*	@brief	「名前確定」JSON変換クラス
*/
class ConfirmNameJsonConverter : JsonConverterBase<JSONRecvPacket_ConfirmName>
{
	// メンバ関数
public:
	// JSON文字列からパケットに変換する
	JSONRecvPacket_ConfirmName from_json_to_packet(const std::string& json) const;
	// パケットからJSON文字列に変換する
	std::string from_packet_to_json(const JSONRecvPacket_ConfirmName& pkt) const;
};