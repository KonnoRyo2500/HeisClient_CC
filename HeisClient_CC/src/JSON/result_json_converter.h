/**
*	@file		result_json_converter.h
*	@brief		heis 「結果」JSON変換クラス
*	@author		Ryo Konno
*	@details	「結果」JSONの変換を行うクラス。
*/

#pragma once

#include "json_converter_base.h"
#include "JSON_data_packet.h"

/**
*	@brief	「結果」JSON変換クラス
*/
class ResultJsonConverter : JsonConverterBase<JSONRecvPacket_Result>
{
	// メンバ関数
public:
	// JSON文字列からパケットに変換する
	JSONRecvPacket_Result from_json_to_packet(std::string json) const;
	// パケットからJSON文字列に変換する
	std::string from_packet_to_json(JSONRecvPacket_Result pkt) const;
};