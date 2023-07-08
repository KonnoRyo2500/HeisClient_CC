/**
*	@file		json_converter_base.h
*	@brief		heis JSON変換ベースクラス
*	@author		Ryo Konno
*	@details	各種JSON解析クラス(JSON <-> パケットの相互変換を行うクラス)のベースクラス。
*/

#pragma once

#include "picojson.h"
#include "common.h"

/**
*	@brief	JSON解析ベースクラス
*	@tparam PacketType 取り扱うJSONパケットの型
*/
template <typename PacketType>
class JsonConverterBase
{
	// メンバ関数
public:
	// JSON文字列からパケットに変換する
	virtual PacketType from_json_to_packet(const std::string& json) const = 0;
	// パケットからJSON文字列に変換する
	virtual std::string from_packet_to_json(const PacketType& pkt) const = 0;

protected:
	// JSON文字列をパースする
	picojson::object parse_json(const std::string& json) const;
};

/**
*	@brief	JSONをパースする
*	@tparam PacketType 取り扱うJSONパケットの型
*	@param[in] json パース対象のJSON
*	@return picojson::object パースされたJSONオブジェクト
*/
template <typename PacketType>
picojson::object JsonConverterBase<PacketType>::parse_json(const std::string& json) const
{
	picojson::value parsed_JSON_val;
	std::string errmsg = picojson::parse(parsed_JSON_val, json);
	if (errmsg.size() > 0) {
		throw std::runtime_error(cc_common::format("JSONのパースに失敗しました(エラー内容: %s)", errmsg.c_str()));
	}

	// JSON -> データへの変換を行いやすくするため，picojson::value -> picojson::objectへの変換を行う
	return parsed_JSON_val.get<picojson::object>();
}