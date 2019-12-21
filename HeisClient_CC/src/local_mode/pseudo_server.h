// heis ローカルモード用疑似サーバクラス
// Author: Ryo Konno
#pragma once

#include <string>
#include <vector>
#include "picojson.h"
#include "infantry.h"
#include "local_setting.h"

// クラス名に"Server"とついてはいるが，通信は行わない
// ただし，実際のサーバと同様に，情報のやり取りはJSONで行う
class CPseudoServer
{
	// 構造体，列挙体など

	// メンバ関数
	public:
		// JSON送信(作成)
		std::string send_field_json(const LocalSetting& setting) const;

	private:
		// JSON作成補助
		std::string serialize_JSON_obj(const picojson::object& obj) const;
		picojson::object make_units_elem(const CInfantry* infantry) const;
		picojson::array make_units_JSON_array() const;

		// 兵士のIDを作成
		std::string make_infantry_id(const std::string& team_name, const int infantry_serial_number) const;

	// メンバ変数
	private:
};