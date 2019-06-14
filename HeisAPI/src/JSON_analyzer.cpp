// heis JSON解析クラス
// Author: Ryo Konno

#include "JSON_analyzer.h"

/* public関数 */

/*
	コンストラクタ
	引数なし
*/
CJSONAnalyzer::CJSONAnalyzer()
	: m_sck(new CSocket("192.168.1.101", 8823))
	, m_latest_JSON_type(RecvJSONType_NoJSONReceived)
{
	// TODO: サーバのIPアドレスとポート番号を設定ファイルから取得するようにする
}

/*
	デストラクタ
	引数なし
*/
CJSONAnalyzer::~CJSONAnalyzer()
{
	delete m_sck;
	m_sck = NULL;
}

/*
	最後に受信したJSONの種類を取得する関数
	引数なし
	返り値: CJSONAnalyzer::RecvJSONType 最後に受信したJSONの種類
*/
CJSONAnalyzer::RecvJSONType CJSONAnalyzer::get_received_JSON_type()
{
	return m_latest_JSON_type;
}

/*
	「行動」JSONをサーバに送信する関数
	引数1: const CCommander::JSONSendData_Action& action_data 「行動」JSONを作成するためのデータ
	返り値なし
*/
void CJSONAnalyzer::send_action_data(const CCommander::JSONSendData_Action& action_data) const
{
	picojson::object action_JSON;

	action_JSON.insert(std::make_pair("turn_team", action_data.turn_team));
	action_JSON.insert(std::make_pair("contents", picojson::value(make_contents_array(action_data.contents))));
	send_JSON(action_JSON);
}

/*
	「名前」JSONをサーバに送信する関数
	引数1: const CGameOnline::JSONSendData_Name& name_data 「名前」JSONを作成するためのデータ
	返り値なし
*/
void CJSONAnalyzer::send_name_data(const CGameOnline::JSONSendData_Name& name_data) const
{
	picojson::object name_JSON;

	name_JSON.insert(std::make_pair("team_name", name_data.team_name));
	send_JSON(name_JSON);
}

/* private関数 */

/*
	JSONをサーバから受信する関数
	引数なし
	返り値なし
*/
void CJSONAnalyzer::recv_JSON()
{
	// JSONの受信+パース
	{
		std::string received_JSON = m_sck->recv_from();
		std::string errmsg = picojson::parse(m_parsed_recv_JSON, received_JSON);
		if (errmsg.size() > 0) {
			fprintf(stderr, "JSONのパースでエラーが発生しました(エラー内容: %s)\n", errmsg.c_str());
			throw std::runtime_error("JSONのパースに失敗しました");
		}
	}
	// 受信したJSONの種類を判定
	m_latest_JSON_type = distinguish_recv_JSON_type();
}

/*
	JSONをサーバに送信する関数
	引数: const picojson::object& JSON_obj 送信するJSONオブジェクト
	返り値なし
*/
void CJSONAnalyzer::send_JSON(const picojson::object& JSON_obj) const
{
	m_sck->send_to(picojson::value(JSON_obj).serialize());
}

/*
	「行動」JSONの"contents"配列を作成する関数
	引数1: const std::vector<CInfantry::JSONSendData_Content>& contents_data "contents"配列を作成するのに必要なデータ
	返り値: picojson::array 作成された"contents"配列
*/
picojson::array CJSONAnalyzer::make_contents_array(const std::vector<CInfantry::JSONSendData_Content>& contents_data) const
{
	picojson::array contents_array;

	for (auto& content : contents_data) {
		picojson::object contents_elem;

		// "to"オブジェクトの作成
		{
			picojson::object to_map;

			to_map.insert(std::make_pair("x", picojson::value(static_cast<double>(content.to_x))));
			to_map.insert(std::make_pair("y", picojson::value(static_cast<double>(content.to_y))));
			contents_elem.insert(std::make_pair("to", picojson::value(to_map)));
		}

		// "atk"オブジェクトの作成
		{
			picojson::object atk_map;

			atk_map.insert(std::make_pair("x", picojson::value(static_cast<double>(content.atk_x))));
			atk_map.insert(std::make_pair("y", picojson::value(static_cast<double>(content.atk_y))));
			contents_elem.insert(std::make_pair("atk", picojson::value(atk_map)));
		}

		// 作成した各オブジェクトと，ユニットIDを配列要素として追加
		contents_elem.insert(std::make_pair("unit_id", content.unit_id));
		contents_array.push_back(picojson::value(contents_elem));
	}
	return contents_array;
}

/*
	受信したJSONの種類を判定する関数
	引数なし
	返り値: CJSONAnalyzer::RecvJSONType 受信したJSONの種類
*/
CJSONAnalyzer::RecvJSONType CJSONAnalyzer::distinguish_recv_JSON_type() const
{
	return RecvJSONType_Message;
}

/*
	受信したJSONの種類と要求しているJSONの種類が一致しているかを判定する関数
	引数1: const RecvJSONType req_JSON_type 要求しているJSONの種類
	返り値なし
*/
void CJSONAnalyzer::validate_JSON_type(const RecvJSONType req_JSON_type) const
{
	if (req_JSON_type != m_latest_JSON_type) {
		// TODO: 例外を投げるようにする?(動作を再考する)
		fprintf(stderr, "警告: 受信したJSONの種類と要求しているJSONの種類が一致しません\n");
	}
}