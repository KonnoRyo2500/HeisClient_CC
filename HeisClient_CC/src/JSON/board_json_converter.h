/**
*	@file		board_json_converter.h
*	@brief		heis 「盤面」JSON変換クラス
*	@author		Ryo Konno
*	@details	「盤面」JSONの変換を行うクラス。
*/

#pragma once

#include "json_converter_base.h"
#include "JSON_data_packet.h"

/**
*	@brief	「盤面」JSON変換クラス
*/
class BoardJsonConverter : public JsonConverterBase<JSONRecvPacket_Board>
{
	// メンバ関数
public:
	// JSON文字列からパケットに変換する
	JSONRecvPacket_Board from_json_to_packet(std::string json) const;
	// パケットからJSON文字列に変換する
	std::string from_packet_to_json(JSONRecvPacket_Board pkt) const;
};