/**
*	@file		result_json_converter.cpp
*	@brief		heis 「結果」JSON変換クラス
*	@author		Ryo Konno
*	@details	「結果」JSONの変換を行うクラス。
*/

#include "result_json_converter.h"

/**
*	@brief 「結果」JSONから「結果」パケットに変換する
*	@param[in] json 変換対象の「結果」JSON
*	@return std::string 変換結果の「結果」パケット
*/
JSONRecvPacket_Result ResultJsonConverter::from_json_to_packet(std::string json) const
{
	JSONRecvPacket_Result result_pkt;
	picojson::object root_obj = parse_json(json);

	// "result"配列の値を取得
	std::vector<ResultArrayElem> result;
	picojson::array result_array = root_obj["result"].get<picojson::array>();
	for (auto& val : result_array) {
		ResultArrayElem elem;
		picojson::object obj = val.get<picojson::object>();

		// "unit_id"は省略されうるため、JSONにキーが存在するかどうかを判定してから取得する
		if (obj.find("unit_id") != obj.end()) {
			elem.unit_id.set_value(obj["unit_id"].get<std::string>());
		}
		elem.error.set_value(obj["error"].get<std::string>());

		result.push_back(elem);
	}
	result_pkt.result.set_value(result);

	return result_pkt;
}

/**
*	@brief 「結果」パケットから「結果」JSONに変換する
*	@param[in] pkt 変換対象の「結果」パケット
*	@return std::string 変換結果の「結果」JSON
*/
std::string ResultJsonConverter::from_packet_to_json(JSONRecvPacket_Result pkt) const
{
	// クライアント側で「結果」パケットから「結果」JSONに変換する必要がないため、Do Nothing
	// 変換する必要が生じたら適宜実装する
	return "";
}