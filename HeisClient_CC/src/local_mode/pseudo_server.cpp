/**
*	@file		pseudo_server.cpp
*	@brief		heis ローカルモード用疑似サーバクラス
*	@author		Ryo Konno
*	@details	ローカルモードにおいて，サーバの動作を模擬する．
*/
#include "pseudo_server.h"
#include "const_val.h"
#include "board.h"
#include "common.h"

/* public関数 */

/**
*	@brief 「盤面」JSONを作成する関数
*	@param[in] setting ローカルモード設定
*	@return std::string 「盤面」JSON
*/
std::string CPseudoServer::send_board_json(const LocalSetting& setting) const
{
	picojson::object board_json_obj;
	static int turn_count = 1;
	static std::string turn_team = setting.first_turn_team;

	board_json_obj.insert(std::make_pair("width", static_cast<double>(setting.board_width)));
	board_json_obj.insert(std::make_pair("height", static_cast<double>(setting.board_height)));
	board_json_obj.insert(std::make_pair("count", static_cast<double>(turn_count)));
	board_json_obj.insert(std::make_pair("finished", judge_finished(setting)));
	board_json_obj.insert(std::make_pair("turn_team", turn_team));
	board_json_obj.insert(std::make_pair("players", make_players_array(setting)));
	board_json_obj.insert(std::make_pair("units", make_units_JSON_array(setting, turn_count)));
	
	// 次の「盤面」JSONを送信するための準備
	turn_count++;
	toggle_turn_team(setting, turn_team);

	const std::string board_json = serialize_JSON_obj(board_json_obj);
	g_system_log->write_log(CLog::LogLevel_InvisibleInfo, cc_common::format("疑似サーバが「盤面」JSONを送信しました: %s",
		board_json.c_str()));

	return board_json;
}

/* private関数 */

/**
*	@brief JSONオブジェクトを文字列化する関数
*	@param[in] obj JSONオブジェクト
*	@return std::string objを文字列化した文字列
*/
// TODO: この関数と全く同じ関数がJSON_analyzer.cppにも定義されているので，まとめたい
std::string CPseudoServer::serialize_JSON_obj(const picojson::object& obj) const
{
	return picojson::value(obj).serialize();
}

/**
*	@brief 「盤面」JSONの"units"配列を作成する関数
*	@param[in] setting ローカルモード設定
*	@param[in] turn_count ターンの回数
*	@return picojson::array 「盤面」JSONの"units"配列
*/
picojson::array CPseudoServer::make_units_JSON_array(const LocalSetting& setting, const int turn_count) const
{
	return (turn_count > 1 ? make_units_JSON_array_from_board() : make_initial_units_JSON_array(setting));
}

/**
*	@brief 最初のターンにおける，「盤面」JSONの"units"配列を作成する関数
*	@param[in] setting ローカルモード設定
*	@return picojson::array 「盤面」JSONの"units"配列
*/
picojson::array CPseudoServer::make_initial_units_JSON_array(const LocalSetting& setting) const
{
	picojson::array units;
	int serial_num = 0;

	// 自チーム兵士の初期配置
	for (auto init_pos : setting.my_team_init_pos) {
		units.push_back(picojson::value(make_initial_units_elem(setting.my_team_name, init_pos, serial_num)));
		serial_num++;
	}
	serial_num = 0;
	// 敵チーム兵士の初期配置
	for (auto init_pos : setting.enemy_team_init_pos) {
		units.push_back(picojson::value(make_initial_units_elem(setting.enemy_team_name, init_pos, serial_num)));
		serial_num++;
	}

	return units;
}

/**
*	@brief 最初のターンにおける，"units"配列の要素1つ分のJSONオブジェクトを作成する関数
*	@param[in] team_name チーム名
*	@param[in] init_pos 初期位置
*	@param[in] infantry_serial_num 兵士のIDにつける番号
*	@return picojson::object settingで指定した初期配置に基づく
*/
picojson::object CPseudoServer::make_initial_units_elem(const std::string& team_name, const BoardPosition& init_pos, const int infantry_serial_num) const
{
	picojson::object units_elem_obj;
	picojson::object locate_obj;

	locate_obj.insert(std::make_pair("x", static_cast<double>(init_pos.x)));
	locate_obj.insert(std::make_pair("y", static_cast<double>(init_pos.y)));

	units_elem_obj.insert(std::make_pair("type", "heizu"));
	units_elem_obj.insert(std::make_pair("unit_id", make_infantry_id(team_name, infantry_serial_num)));
	units_elem_obj.insert(std::make_pair("locate", locate_obj));
	units_elem_obj.insert(std::make_pair("hp", static_cast<double>(INFANTRY_INITIAL_HP)));
	units_elem_obj.insert(std::make_pair("team", team_name));
	return units_elem_obj;
}

/**
*	@brief 「盤面」JSONの"units"配列を盤面から作成する関数
*	@return picojson::array 「盤面」JSONの"units"配列
*/
picojson::array CPseudoServer::make_units_JSON_array_from_board() const
{
	picojson::array units_JSON_array;
	CBoard* board = CBoard::get_instance();

	// 盤面上にいる全兵士の情報を，そのまま"units"配列に入れる
	for (int x = 0; x < board->get_width(); x++) {
		for (int y = 0; y < board->get_height(); y++) {
			CInfantry* infantry = board->get_infantry(BoardPosition(x, y));

			if (infantry != NULL) {
				units_JSON_array.push_back(picojson::value(make_units_elem_from_board(infantry)));
			}
		}
	}
	return units_JSON_array;
}

/**
*	@brief 盤面上の兵士の情報から，"units"配列の要素1つ分のJSONオブジェクトを作成する関数
*	@param[in] infantry 兵士
*	@return picojson::object infantryのデータから作成されたJSONオブジェクト
*/
picojson::object CPseudoServer::make_units_elem_from_board(const CInfantry* infantry) const
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

/**
*	@brief 対戦が終了しているかどうかを判定し，「盤面」JSONの"finished"フィールドのための真偽値を返す関数
*	@param[in] setting ローカルモード設定
*	@return bool 「盤面」JSONの"finished"フィールドの値
*/
bool CPseudoServer::judge_finished(const LocalSetting& setting) const
{
	CBoard* board = CBoard::get_instance();
	bool is_my_team_alive = false, is_enemy_team_alive = false;

	// 最初のターンは，必ず継続させる
	if (board->get_width() <= 0 && board->get_height() <= 0) {
		return false;
	}

	// 盤面上にいる全兵士の情報を，そのまま"units"配列に入れる
	for (int x = 0; x < board->get_width(); x++) {
		for (int y = 0; y < board->get_height(); y++) {
			CInfantry* infantry = board->get_infantry(BoardPosition(x, y));

			// infantry != NULLとして下のif文をネストさせるとネストが深くなるので，このような書き方をした
			if (infantry == NULL) {
				continue;
			}
			if (infantry->get_team_name() == setting.my_team_name) {
				is_my_team_alive = true;
			}
			else if (infantry->get_team_name() == setting.enemy_team_name) {
				is_enemy_team_alive = true;
			}
		}
	}

	return !(is_my_team_alive && is_enemy_team_alive);
}

/**
*	@brief 「盤面」JSONの"players"フィールドに入るJSON配列を構築する関数
*	@param[in] setting ローカルモード設定
*	@return picojson::array 「盤面」JSONの"players"フィールドの値
*/
picojson::array CPseudoServer::make_players_array(const LocalSetting& setting) const
{
	picojson::array players;

	players.push_back(picojson::value(setting.my_team_name));
	players.push_back(picojson::value(setting.enemy_team_name));

	return players;
}

/**
*	@brief 「盤面」JSONの"turn_team"に入るチーム名を切り替える関数
*	@param[in] setting ローカルモード設定
*	@param[out] turn_team チーム名(敵 <-> 味方が入れ替わる)
*/
void CPseudoServer::toggle_turn_team(const LocalSetting& setting, std::string& turn_team) const
{
	turn_team = (turn_team == setting.my_team_name ? setting.enemy_team_name : setting.my_team_name);
}

/**
*	@brief ローカルモード用の兵士IDを作成する関数
*	@param[in] team_name 兵士のチーム名
*	@param[in] infantry_serial_number IDの数字部分(兵士に付けられる連番)
*	@return std::string 兵士のID
*	@remark 兵士のコンストラクタに与えるIDを生成するための関数なので，CInfantry型のオブジェクトは渡せない
*/
std::string CPseudoServer::make_infantry_id(const std::string& team_name, const int infantry_serial_number) const
{
	char id[10] = {0};

	// IDの命名法はオンラインモードの仕様に則り，"(チーム名の先頭2文字)+(通し番号)"とする
	snprintf(id, sizeof(id) / sizeof(char), "%s%02d", team_name.substr(0, 2).c_str(), infantry_serial_number);
	return std::string(id);
}