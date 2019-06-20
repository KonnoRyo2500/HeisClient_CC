// heis JSON解析クラス
// Author: Ryo Konno

#include "JSON_analyzer.h"

/* public関数 */

/*
	コンストラクタ
	引数なし
*/
CJSONAnalyzer::CJSONAnalyzer()
	: m_analyzed_JSON_kind(JSONKind_NoJSONAnalyzed)
{
	// 処理なし
}

/*
	デストラクタ
	引数なし
*/
CJSONAnalyzer::~CJSONAnalyzer()
{
	// 処理なし
}

/*
	最後に解析したJSONの種類を取得する関数
	引数なし
	返り値: CJSONAnalyzer::JSONKind 最後に解析したJSONの種類
*/
CJSONAnalyzer::JSONKind CJSONAnalyzer::get_analyzed_JSON_kind()
{
	return m_analyzed_JSON_kind;
}

/*
	「行動」パケットから「行動」JSONを作成する関数
	引数1: const CCommander::JSONSendData_Action& action_pkt 「行動」パケット
	返り値: std::string 「行動」JSON
*/
std::string CJSONAnalyzer::create_action_JSON(const CCommander::JSONSendData_Action& action_pkt) const
{
	picojson::object action_JSON;

	// JSON作成
	action_JSON.insert(std::make_pair("turn_team", action_pkt.turn_team));
	action_JSON.insert(std::make_pair("contents", picojson::value(make_contents_JSON(action_pkt.contents))));

	return serialize_JSON_obj(action_JSON);
}

/*
	「名前」パケットから「名前」JSONを作成する関数
	引数1: const CGameOnline::JSONSendData_Name& name_pkt 「名前」パケット
	返り値: std::string 「名前」JSON
*/
std::string CJSONAnalyzer::create_name_JSON(const CGameOnline::JSONSendData_Name& name_pkt) const
{
	picojson::object name_JSON;

	// JSON作成
	name_JSON.insert(std::make_pair("team_name", name_pkt.team_name));

	return serialize_JSON_obj(name_JSON);
}

/*
	与えられた「名前確定」JSONから，「名前確定」パケットを作成する関数
	引数1: const std::string& name_decided_JSON 「名前確定」JSON
	返り値: CGameOnline::JSONRecvData_NameDecided 「名前確定」パケット
*/
CGameOnline::JSONRecvData_NameDecided CJSONAnalyzer::create_name_decided_pkt(const std::string& name_decided_JSON)
{
	// JSONの解析+種別チェック
	analyze_JSON(name_decided_JSON);
	validate_JSON_kind(JSONKind_NameDecided);

	// JSONからパケット作成
	CGameOnline::JSONRecvData_NameDecided name_pkt;

	name_pkt.your_team = get_not_number_val_from_JSON_obj<std::string>("your_team", m_analyzed_JSON_root_obj);
	return name_pkt;
}

/*
	与えられた「結果」JSONから，「結果」パケットを作成する関数
	引数1: const std::string& result_JSON 「結果」JSON
	返り値: CGameOnline::JSONRecvData_Result 「結果」パケット
*/
CGameOnline::JSONRecvData_Result CJSONAnalyzer::create_result_pkt(const std::string& result_JSON)
{
	// JSONの解析+種別チェック
	analyze_JSON(result_JSON);
	validate_JSON_kind(JSONKind_Result);

	// JSONからパケット作成
	CGameOnline::JSONRecvData_Result result_pkt;

	result_pkt.result = make_result_array(get_not_number_val_from_JSON_obj<picojson::array>("result", m_analyzed_JSON_root_obj));
	return result_pkt;
}

/*
	与えられた「メッセージ」JSONから，「メッセージ」パケットを作成する関数
	引数1: const std::string& message_JSON 「メッセージ」JSON
	返り値: CGameOnline::JSONRecvData_Message 「メッセージ」パケット
*/
CGameOnline::JSONRecvData_Message CJSONAnalyzer::create_message_pkt(const std::string& message_JSON)
{
	// JSONの解析+種別チェック
	analyze_JSON(message_JSON);
	validate_JSON_kind(JSONKind_Message);

	// JSONからパケット作成
	CGameOnline::JSONRecvData_Message message_pkt;

	message_pkt.message = get_not_number_val_from_JSON_obj<std::string>("message", m_analyzed_JSON_root_obj);
	return message_pkt;
}

/*
	与えられた「盤面」JSONから，「盤面」パケットを作成する関数
	引数1: const std::string& field_JSON 「盤面」JSON
	返り値: CField::JSONRecvData_Field 「盤面」パケット
*/
CField::JSONRecvData_Field CJSONAnalyzer::create_field_pkt(const std::string& field_JSON)
{
	// JSONの解析+種別チェック
	analyze_JSON(field_JSON);
	validate_JSON_kind(JSONKind_Field);

	// JSONからデータ作成
	CField::JSONRecvData_Field field_pkt;

	field_pkt.width = get_number_val_from_JSON_obj<int16_t>("width", m_analyzed_JSON_root_obj);
	field_pkt.height = get_number_val_from_JSON_obj<int16_t>("height", m_analyzed_JSON_root_obj);
	field_pkt.turn_team = get_not_number_val_from_JSON_obj<std::string>("turn_team", m_analyzed_JSON_root_obj);
	field_pkt.finished = get_not_number_val_from_JSON_obj<bool>("finished", m_analyzed_JSON_root_obj);
	field_pkt.count = get_number_val_from_JSON_obj<uint32_t>("count", m_analyzed_JSON_root_obj);
	field_pkt.players = make_players_array(get_not_number_val_from_JSON_obj<picojson::array>("players", m_analyzed_JSON_root_obj));
	field_pkt.units = make_units_array(get_not_number_val_from_JSON_obj<picojson::array>("units", m_analyzed_JSON_root_obj));
	return field_pkt;
}

/* private関数 */

/*
	与えられたJSONを解析(パース+種類判定)する関数
	引数1: const std::string& src_JSON 解析元のJSON
	返り値なし
	例外: JSONのパースに失敗したとき
*/
void CJSONAnalyzer::analyze_JSON(const std::string& src_JSON)
{
	// JSONのパース
	{
		picojson::value parsed_JSON_val;
		std::string errmsg = picojson::parse(parsed_JSON_val, src_JSON);

		if (errmsg.size() > 0) {
			fprintf(stderr, "JSONのパースでエラーが発生しました(エラー内容: %s)\n", errmsg.c_str());
			throw std::runtime_error("JSONのパースに失敗しました");
		}
		// JSON -> データへの変換を行いやすくするため，picojson::value -> picojson::objectへの変換を行う
		m_analyzed_JSON_root_obj = parsed_JSON_val.get<picojson::object>();
	}
	// パースしたJSONの種類を判定
	m_analyzed_JSON_kind = distinguish_analyzed_JSON_kind();
}

/*
	JSONオブジェクトを文字列化する関数
	引数1: const picojson::object& obj
	返り値: std::string objを文字列化した文字列 
*/
std::string CJSONAnalyzer::serialize_JSON_obj(const picojson::object& obj) const
{
	return picojson::value(obj).serialize();
}

/*
	「行動」JSONの"contents"配列を作成する関数
	引数1: const std::vector<CInfantry::JSONSendData_Content>& contents_data "contents"配列を作成するのに必要なデータ
	返り値: picojson::array 作成された"contents"配列
*/
picojson::array CJSONAnalyzer::make_contents_JSON(const std::vector<CInfantry::JSONSendData_Content>& contents_data) const
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
	「結果」JSONの"result"配列から，内部処理用データを作成する関数
	引数1: const picojson::array& result_array "result"配列
	返り値: std::vector<CGameOnline::JSONRecvData_ResultElem> "result"配列から作成されたデータ
*/
std::vector<CGameOnline::JSONRecvData_ResultElem> CJSONAnalyzer::make_result_array(const picojson::array& result_array) const
{
	std::vector<CGameOnline::JSONRecvData_ResultElem> result_data;

	for (auto& result_array_val : result_array) {
		CGameOnline::JSONRecvData_ResultElem result_data_elem;
		picojson::object elem_obj = result_array_val.get<picojson::object>();

		result_data_elem.unit_id = get_not_number_val_from_JSON_obj<std::string>("unit_id", elem_obj);
		result_data_elem.error = get_not_number_val_from_JSON_obj<std::string>("error", elem_obj);
		result_data.push_back(result_data_elem);
	}
	return result_data;
}

/*
	「盤面」JSONの"players"配列から内部処理用データを作成する関数
	引数なし
	返り値: std::vector<std::string> "players"配列から作成されたデータ
*/
std::vector<std::string> CJSONAnalyzer::make_players_array(const picojson::array& players_array) const
{
	std::vector<std::string> players_data;

	for (auto& players_array_val : players_array) {
		players_data.push_back(players_array_val.get<std::string>());
	}
	return players_data;
}

/*
	「盤面」JSONの"units"配列から内部処理用データを作成する関数
	引数なし
	返り値: std::vector<CField::JSONRecvData_Unit> "units"配列から作成されたデータ
*/
std::vector<CField::JSONRecvData_Unit> CJSONAnalyzer::make_units_array(const picojson::array& units_array) const
{
	std::vector<CField::JSONRecvData_Unit> units_data;

	for (auto& units_array_val : units_array) {
		CField::JSONRecvData_Unit unit;
		picojson::object elem_obj = units_array_val.get<picojson::object>();

		unit.hp = get_number_val_from_JSON_obj<int8_t>("hp", elem_obj);
		unit.unit_id = get_not_number_val_from_JSON_obj<std::string>("unit_id", elem_obj);
		unit.type = get_not_number_val_from_JSON_obj<std::string>("type", elem_obj);
		unit.team = get_not_number_val_from_JSON_obj<std::string>("team", elem_obj);
		unit.locate = make_locate_object(get_not_number_val_from_JSON_obj<picojson::object>("locate", elem_obj));
		units_data.push_back(unit);
	}
	return units_data;
}

/*
	「盤面」JSONの"locate"オブジェクトから内部処理用データを作成する関数
	引数なし
	返り値: CField::JSONRecvData_Locate "locate"オブジェクトから作成されたデータ
*/
CField::JSONRecvData_Locate CJSONAnalyzer::make_locate_object(const picojson::object& locate_obj) const
{
	CField::JSONRecvData_Locate locate_data;

	locate_data.x = get_number_val_from_JSON_obj<int16_t>("x", locate_obj);
	locate_data.y = get_number_val_from_JSON_obj<int16_t>("y", locate_obj);
	return locate_data;
}

/*
	解析したJSONの種類を判定する関数
	引数なし
	返り値: CJSONAnalyzer::JSONKind 解析したJSONの種類
*/
CJSONAnalyzer::JSONKind CJSONAnalyzer::distinguish_analyzed_JSON_kind() const
{
	return JSONKind_Message;
}

/*
	解析したJSONの種類と要求しているJSONの種類が一致しているかを判定する関数
	引数1: const JSONKind req_JSON_kind 要求しているJSONの種類
	返り値なし
*/
void CJSONAnalyzer::validate_JSON_kind(const JSONKind req_JSON_kind) const
{
	if (req_JSON_kind != m_analyzed_JSON_kind) {
		// TODO: 例外を投げるようにする?(動作を再考する)
		fprintf(stderr, "警告: 直前に解析したJSONの種類と要求しているJSONの種類が一致しません\n");
	}
}