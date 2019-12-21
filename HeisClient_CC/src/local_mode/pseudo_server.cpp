// heis ローカルモード用疑似サーバクラス
// Author: Ryo Konno

#include "pseudo_server.h"
#include "const_val.h"
#include "field.h"

/* public関数 */

/*
	「盤面」JSONを作成する関数
	引数1: const LocalSetting& setting ローカルモード設定
	返り値: std::string 「盤面」JSON
*/
std::string CPseudoServer::send_field_json(const LocalSetting& setting) const
{
	picojson::object field_json_obj;
	
	return serialize_JSON_obj(field_json_obj);
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

	locate_obj.insert(std::make_pair("x", static_cast<double>(infantry->get_position().x)));
	locate_obj.insert(std::make_pair("y", static_cast<double>(infantry->get_position().y)));

	units_elem_obj.insert(std::make_pair("type", "heizu"));
	units_elem_obj.insert(std::make_pair("unit_id", infantry->get_id()));
	units_elem_obj.insert(std::make_pair("locate", locate_obj));
	units_elem_obj.insert(std::make_pair("hp", static_cast<double>(infantry->get_hp())));
	units_elem_obj.insert(std::make_pair("team", infantry->get_team_name()));
	return units_elem_obj;
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
			CInfantry* infantry = field->get_infantry(FieldPosition(x, y));

			if (infantry != NULL) {
				units_JSON_array.push_back(picojson::value(make_units_elem(infantry)));
			}
		}
	}
	return units_JSON_array;
}

/*
	ローカルモード用の兵士IDを作成する関数
	引数1: const std::string& team_name 兵士のチーム名
	引数2: const int infantry_serial_number IDの数字部分(兵士に付けられる連番)
	返り値: std::string 兵士のID
	備考: 兵士のコンストラクタに与えるIDを生成するための関数なので，CInfantry型のオブジェクトは渡せない
*/
std::string CPseudoServer::make_infantry_id(const std::string& team_name, const int infantry_serial_number) const
{
	char id[10] = {0};

	// IDの命名法はオンラインモードの仕様に則り，"(チーム名の先頭2文字)+(通し番号)"とする
	snprintf(id, sizeof(id) / sizeof(char), "%s%02d", team_name.substr(0, 2).c_str(), infantry_serial_number);
	return std::string(id);
}