/**
*	@file		action_json_converter.cpp
*	@brief		heis 「行動」JSON変換クラス
*	@author		Ryo Konno
*	@details	「行動」JSONの変換を行うクラス。
*/

#include "action_json_converter.h"

/**
*	@brief 「行動」JSONから「行動」パケットに変換する
*	@param[in] json 変換対象の「行動」JSON
*	@return std::string 変換結果の「行動」パケット
*/
JSONSendPacket_Action ActionJsonConverter::from_json_to_packet(const std::string& json) const
{
	// クライアント側で「行動」JSONから「行動」パケットに変換する必要がないため、Do Nothing
	// 変換する必要が生じたら適宜実装する
	return JSONSendPacket_Action();
}

/**
*	@brief 「盤面」パケットから「盤面」JSONに変換する
*	@param[in] pkt 変換対象の「盤面」パケット
*	@return std::string 変換結果の「盤面」JSON
*/
std::string ActionJsonConverter::from_packet_to_json(const JSONSendPacket_Action& pkt) const
{
	picojson::object action_obj;

	// パケットから直接変換できる要素をJSONに追加
	action_obj.insert(std::make_pair("turn_team", pkt.turn_team.get_value()));

	// "contents"配列を作成
	picojson::array contents;
	for (auto& content : pkt.contents.get_value()) {
		picojson::object contents_obj;

		// "to"オブジェクトの作成
		{
			picojson::object to_obj;

			to_obj.insert(std::make_pair("x", picojson::value(static_cast<double>(content.to_x.get_value()))));
			to_obj.insert(std::make_pair("y", picojson::value(static_cast<double>(content.to_y.get_value()))));
			contents_obj.insert(std::make_pair("to", picojson::value(to_obj)));
		}

		// "atk"オブジェクトの作成
		{
			picojson::object atk_obj;

			atk_obj.insert(std::make_pair("x", picojson::value(static_cast<double>(content.atk_x.get_value()))));
			atk_obj.insert(std::make_pair("y", picojson::value(static_cast<double>(content.atk_y.get_value()))));
			contents_obj.insert(std::make_pair("atk", picojson::value(atk_obj)));
		}

		// 作成した各オブジェクトと，ユニットIDを配列要素として追加
		contents_obj.insert(std::make_pair("unit_id", content.unit_id.get_value()));
		contents.push_back(picojson::value(contents_obj));
	}
	action_obj.insert(std::make_pair("contents", "x"));

	return picojson::value(action_obj).serialize();
}