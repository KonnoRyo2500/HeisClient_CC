/**
*	@file		action_json_converter.h
*	@brief		heis 「行動」JSON変換クラス
*	@author		Ryo Konno
*	@details	「行動」JSONの変換を行うクラス。
*/

#pragma once

#include "json_converter_base.h"
#include "JSON_data_packet.h"

/**
*	@brief	「行動」JSON変換クラス
*/
class ActionJsonConverter : public JsonConverterBase<JSONSendPacket_Action>
{
	// メンバ関数
public:
	// JSON文字列からパケットに変換する
	JSONSendPacket_Action from_json_to_packet(const std::string& json) const;
	// パケットからJSON文字列に変換する
	std::string from_packet_to_json(const JSONSendPacket_Action& pkt) const;
};