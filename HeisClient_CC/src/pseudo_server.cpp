// heis ローカルモード用疑似サーバクラス
// Author: Ryo Konno

#include "pseudo_server.h"
#include "const_val.h"
#include "field.h"

/* public関数 */

/*
	名前要求を行う「メッセージ」JSONを作成する関数
	引数なし
	返り値: std::string 名前要求を行う「メッセージ」JSON
*/
std::string CPseudoServer::send_name_req_message_json() const
{
	picojson::object message_json_obj;

	// TODO: 名前要求の際に入る正確なメッセージが判明したら，それを入れるようにする
	message_json_obj.insert(std::make_pair("message", "name request"));
	return serialize_JSON_obj(message_json_obj);
}

/*
	「名前確定」JSONを作成する関数
	引数なし
	返り値: std::string 「名前確定」JSON
*/
std::string CPseudoServer::send_name_decided_json() const
{
	picojson::object name_decide_json_obj;

	// 具体的なチーム名はCGameLocalで定義されているので，ここではチーム名を考慮する必要はない
	name_decide_json_obj.insert(std::make_pair("your_team", "XXX"));
	return serialize_JSON_obj(name_decide_json_obj);
}

/*
	最初に送信される「盤面」JSONを作成する関数
	引数なし
	返り値: std::string 最初に送信される「盤面」JSON
*/
std::string CPseudoServer::send_initial_field_json() const
{
	picojson::object initial_field_json_obj;

	initial_field_json_obj.insert(std::make_pair("turn_team", "XXX"));
	initial_field_json_obj.insert(std::make_pair("players", picojson::array()));
	initial_field_json_obj.insert(std::make_pair("finished", false));
	initial_field_json_obj.insert(std::make_pair("count", 0.0));
	// ローカルモードでは，CGameLocal側でゲームの流れの制御を行うため，以下の値を除き，このJSONからの値を無視する
	// そのため，以下で設定する値以外には，適当な固定値を入れておく
	initial_field_json_obj.insert(std::make_pair("width", static_cast<double>(LocalFieldSize_Width)));
	initial_field_json_obj.insert(std::make_pair("height", static_cast<double>(LocalFieldSize_Height)));
	initial_field_json_obj.insert(std::make_pair("units", make_initial_units_JSON_array()));
	return serialize_JSON_obj(initial_field_json_obj);
}

/*
	「盤面」JSONを作成する関数
	引数なし
	返り値: std::string 「盤面」JSON
*/
std::string CPseudoServer::send_field_json() const
{
	picojson::object field_json_obj;

	field_json_obj.insert(std::make_pair("turn_team", "XXX"));
	field_json_obj.insert(std::make_pair("players", picojson::array()));
	field_json_obj.insert(std::make_pair("finished", false));
	field_json_obj.insert(std::make_pair("count", 0.0));
	// ローカルモードでは，CGameLocal側でゲームの流れの制御を行うため，以下の値を除き，このJSONからの値を無視する
	// そのため，以下で設定する値以外には，適当な固定値を入れておく
	field_json_obj.insert(std::make_pair("width", static_cast<double>(LocalFieldSize_Width)));
	field_json_obj.insert(std::make_pair("height", static_cast<double>(LocalFieldSize_Height)));
	field_json_obj.insert(std::make_pair("units", make_units_JSON_array()));
	return serialize_JSON_obj(field_json_obj);
}

/*
	「結果」JSONを作成する関数
	引数なし
	返り値: std::string 「結果」JSON
*/
std::string CPseudoServer::send_result_json() const
{
	picojson::object result_json_obj;

	result_json_obj.insert(std::make_pair("result", picojson::array()));
	return serialize_JSON_obj(result_json_obj);
}

/*
	「名前」JSONを受け取る関数
	引数1: const std::string& name_json 「名前」JSON
	返り値なし
*/
void CPseudoServer::recv_name_json(const std::string& name_json) const
{
	// 処理なし(受け取ったJSONは捨てる)
}

/*
	「行動」JSONを受け取る関数
	引数1: const std::string& name_json 「行動」JSON
	返り値なし
*/
void CPseudoServer::recv_action_json(const std::string& action_json) const
{
	// 処理なし(受け取ったJSONは捨てる)
}

/* private関数 */

/*
	JSONオブジェクトを文字列化する関数
	引数1: const picojson::object& obj
	返り値: std::string objを文字列化した文字列
*/
// TODO: この関数と全く同じ関数がJSON_analyzer.cppにも定義されているので，まとめたい
std::string CPseudoServer::serialize_JSON_obj(const picojson::object& obj) const
{
	return picojson::value(obj).serialize();
}

/*
	兵士の情報から，"units"配列の要素1つ分のJSONオブジェクトを作成する関数
	引数1: const CInfantry* infantry 兵士
	返り値: picojson::object infantryのデータから作成されたJSONオブジェクト
*/
picojson::object CPseudoServer::make_units_elem(const CInfantry* infantry) const
{
	picojson::object units_elem_obj;
	picojson::object locate_obj;

	locate_obj.insert(std::make_pair("x", static_cast<double>(infantry->get_x_position())));
	locate_obj.insert(std::make_pair("y", static_cast<double>(infantry->get_y_position())));

	units_elem_obj.insert(std::make_pair("type", "heizu"));
	units_elem_obj.insert(std::make_pair("unit_id", infantry->get_id()));
	units_elem_obj.insert(std::make_pair("locate", locate_obj));
	units_elem_obj.insert(std::make_pair("hp", static_cast<double>(infantry->get_hp())));
	units_elem_obj.insert(std::make_pair("team", infantry->get_team_name()));
	return units_elem_obj;
}

/*
	最初に送信される「盤面」JSONの"units"配列を作成する関数
	引数なし
	返り値: picojson::array 最初に送信される「盤面」JSONの"units"配列
*/
picojson::array CPseudoServer::make_initial_units_JSON_array() const
{
	picojson::array initial_units_JSON_array;
	// 兵士を初期配置する領域のサイズ
	const int initial_area_width = 6;
	const int initial_area_height = 6;
	int infantry_serial_num = 0;

	for (int x = 0; x < initial_area_width; x++) {
		for (int y = 0; y < initial_area_height; y++) {
			CInfantry my_infantry(LOCAL_MY_TEAM_NAME, make_infantry_id("te", infantry_serial_num), x, y);
			CInfantry enemy_infantry(LOCAL_ENEMY_TEAM_NAME, make_infantry_id("en", infantry_serial_num), LocalFieldSize_Width - x - 1, LocalFieldSize_Height -  y - 1);
			initial_units_JSON_array.push_back(picojson::value(make_units_elem(&my_infantry)));
			initial_units_JSON_array.push_back(picojson::value(make_units_elem(&enemy_infantry)));
			infantry_serial_num++;
		}
	}

	return initial_units_JSON_array;
}

/*
	「盤面」JSONの"units"配列を作成する関数
	引数なし
	返り値: picojson::array 「盤面」JSONの"units"配列
*/
picojson::array CPseudoServer::make_units_JSON_array() const
{
	picojson::array units_JSON_array;
	CField* field = CField::get_instance();

	// フィールド上にいる全兵士の情報を，そのまま"units"配列に入れる
	for (int x = 0; x < field->get_width(); x++) {
		for (int y = 0; y < field->get_height(); y++) {
			CInfantry* infantry = field->get_infantry(x, y);

			if (infantry != NULL) {
				units_JSON_array.push_back(picojson::value(make_units_elem(infantry)));
			}
		}
	}
	return units_JSON_array;
}

/*
	ローカルモード用の兵士IDを作成する関数
	引数1: const std::string& id_prefix IDの先頭につける文字列
	引数2: const int infantry_serial_number IDの数字部分(兵士に付けられる連番)
	返り値: std::string 兵士のID
*/
std::string CPseudoServer::make_infantry_id(const std::string& id_prefix, const int infantry_serial_number) const
{
	char id[10] = {0};

	snprintf(id, sizeof(id) / sizeof(char), "%s%02d", id_prefix.c_str(), infantry_serial_number);
	return std::string(id);
}