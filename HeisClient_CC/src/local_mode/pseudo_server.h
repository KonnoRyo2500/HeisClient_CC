/**
*	@file		pseudo_server.h
*	@brief		heis ローカルモード用疑似サーバクラス
*	@author		Ryo Konno
*	@details	ローカルモードにおいて，サーバの動作を模擬する．
*/
#pragma once

#include <string>
#include <vector>
#include "picojson.h"
#include "infantry.h"
#include "local_setting.h"
#include "log.h"

extern CLog g_system_log;

// クラス名に"Server"とついてはいるが，通信は行わない
// また，ローカルモードで不要なJSONはやり取りしない(「メッセージ」JSON，「名前決定」JSONなど)
// ただし，実際のサーバと同様に，情報のやり取りはJSONで行う
/**
*	@brief ローカルモード用疑似サーバクラス
*/
class CPseudoServer
{
	// 構造体，列挙体など

	// メンバ関数
	public:
		// JSON送信(作成)
		std::string send_field_json(const LocalSetting& setting) const;

	private:
		// JSONの文字列化
		std::string serialize_JSON_obj(const picojson::object& obj) const;

		// 「盤面」JSON作成補助
		picojson::array make_units_JSON_array(const LocalSetting& setting, const int turn_count) const;
		picojson::array make_initial_units_JSON_array(const LocalSetting& setting) const;
		picojson::object make_initial_units_elem(const std::string& team_name, const FieldPosition& init_pos, const int infantry_serial_num) const;
		picojson::array make_units_JSON_array_from_field() const;
		picojson::object make_units_elem_from_field(const CInfantry* infantry) const;
		bool judge_finished(const LocalSetting& setting) const;
		picojson::array make_players_array(const LocalSetting& setting) const;
		void toggle_turn_team(const LocalSetting& setting, std::string& turn_team) const;

		// 兵士のIDを作成
		std::string make_infantry_id(const std::string& team_name, const int infantry_serial_number) const;

	// メンバ変数
	private:
		// なし
};