// heis JSONデータ管理用パケット
// Author: Ryo Konno
#pragma once

#include <string>
#include <vector>

/* 以下のパケットの構造は，ファイル「兵ズ通信仕様.ods」で定義されているJSONの仕様に基づく */

/* 各種パケットの要素 */

// 「行動」パケット -> contents(配列)の要素
struct ContentsArrayElem {
	std::string unit_id;		// "unit_id"フィールドの値
	int16_t to_x;				// "to"."x"フィールドの値
	int16_t to_y;				// "to"."y"フィールドの値
	int16_t atk_x;				// "atk"."x"フィールドの値
	int16_t atk_y;				// "atk"."y"フィールドの値
};

// 「盤面」パケット -> units -> locate(オブジェクト)のデータ
struct LocateObjData {
	int16_t x;		// "x"(フィールド)
	int16_t y;		// "y"(フィールド)
};

// 「盤面」パケット -> unitsの要素
struct UnitsArrayElem {
	std::string type;				// "type"(フィールド)
	std::string unit_id;			// "unit_id"(フィールド)
	LocateObjData locate;	// "locate"(オブジェクト)
	int8_t hp;						// "hp"(フィールド)
	std::string team;				// "team"(フィールド)
};

// 「結果」パケット -> resultの要素
struct ResultArrayElem {
	std::string unit_id;		// "unit_id"(フィールド)
	std::string error;			// "error"(フィールド)
};

/* サーバに送信するJSONを作成するためのパケット */

// 「名前」パケット
struct JSONSendPacket_Name {
	std::string team_name;		// "team_name"(フィールド)
};

// 「行動」パケット
struct JSONSendPacket_Action {
	std::string turn_team;						// "turn_team"(フィールド)
	std::vector<ContentsArrayElem> contents;	// "contents"(配列)
};

/* サーバから受信したJSONのデータを格納するパケット */

// 「盤面」パケット
struct JSONRecvPacket_Field {
	int16_t width;								// "width"(フィールド)
	int16_t height;								// "height"(フィールド)
	std::string turn_team;						// "turn_team"(フィールド)
	std::vector<std::string> players;			// "players"(配列)
	bool finished;								// "finished"(フィールド)
	uint32_t count;								// "count"(フィールド)
	std::vector<UnitsArrayElem> units;	// "units"(配列)
};

// 「名前確定」パケット
struct JSONRecvPacket_NameDecided {
	std::string your_team;		// "your_team"(フィールド)
};

// 「結果」パケット
struct JSONRecvPacket_Result {
	std::vector<ResultArrayElem> result;	// "result"(配列)
};

// 「メッセージ」パケット
struct JSONRecvPacket_Message {
	std::string message;		// "message"(フィールド)
};