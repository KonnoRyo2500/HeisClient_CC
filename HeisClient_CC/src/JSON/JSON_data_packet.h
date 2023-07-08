/**
*	@file		JSON_data_packet.h
*	@brief		heis JSONデータ管理用パケット
*	@author		Ryo Konno
*	@details	CCとサーバ間でやり取りされるJSONから得られる情報を取り扱うための構造体(パケット)を定義する．
*/
#pragma once

#include <string>
#include <vector>
#include <stdexcept>

/**
*	@struct JsonElement
*	@brief JSONの要素
*/
template <typename T>
struct JsonElement
{
// メンバ関数
public:
	// 値をセットする
	void set_value(T value) {
		this->value = value;
		exists_value = true;
	}
	// 値を取得する
	T get_value() const {
		return value;
	}
	// 値が存在するか
	bool exists() const {
		return exists_value;
	}
	
// メンバ変数
private:
	T value;
	bool exists_value = false;
};

/* 以下のパケットの構造は，通信仕様書で定義されているJSONの仕様に基づく */

/* 各種パケットの要素 */

/**
*	@struct	ContentsArrayElem
*	@brief 「行動」パケット -> contents(配列)の要素
*/
struct ContentsArrayElem {
	//! "unit_id"フィールドの値
	JsonElement<std::string> unit_id;
	//! "to"."x"フィールドの値
	JsonElement<uint16_t> to_x;
	//! "to"."y"フィールドの値
	JsonElement<uint16_t> to_y;
	//! "atk"."x"フィールドの値
	JsonElement<uint16_t> atk_x;
	//! "atk"."y"フィールドの値
	JsonElement<uint16_t> atk_y;
};
 
/**
*	@struct LocateObjData
*	@brief 「盤面」パケット -> units -> locate(オブジェクト)のデータ
*/
struct LocateObjData {
	//! "x"(フィールド)
	JsonElement<uint16_t> x;
	//! "y"(フィールド)
	JsonElement<uint16_t> y;
};

/**
*	@struct UnitsArrayElem
*	@brief 「盤面」パケット -> unitsの要素
*/
struct UnitsArrayElem {
	//! "type"(フィールド)
	JsonElement<std::string> type;
	//! "unit_id"(フィールド)
	JsonElement<std::string> unit_id;
	//! "locate"(オブジェクト)
	JsonElement<LocateObjData> locate;
	//! "hp"(フィールド)
	JsonElement<int8_t> hp;
	//! "team"(フィールド)
	JsonElement<std::string> team;
};

/**
*	@struct ResultArrayElem
*	@brief 「結果」パケット -> resultの要素
*/
struct ResultArrayElem {
	//! "unit_id"(フィールド, 省略可能)
	JsonElement<std::string> unit_id;
	//! "error"(フィールド)
	JsonElement<std::string> error;
};

/* サーバに送信するJSONを作成するためのパケット */
 
/**
*	@struct JSONSendPacket_Name
*	@brief 「名前」パケット
*	@details 「名前」JSONの情報を格納するパケット
*/
struct JSONSendPacket_Name {
	//! "team_name"(フィールド)
	JsonElement<std::string> team_name;
};

/**
*	@struct JSONSendPacket_Action
*	@brief 「行動」パケット
*	@details 「行動」JSONの情報を格納するパケット
*/
struct JSONSendPacket_Action {
	//! "turn_team"(フィールド)
	JsonElement<std::string> turn_team;
	//! "contents"(配列)
	JsonElement<std::vector<ContentsArrayElem>> contents;
};

/* サーバから受信したJSONのデータを格納するパケット */

// 「盤面」パケット
/**
*	@struct JSONRecvPacket_Board
*	@brief 「盤面」パケット
*	@details 「盤面」JSONの情報を格納するパケット
*/
struct JSONRecvPacket_Board {
	//! "width"(フィールド)
	JsonElement<uint16_t> width;
	//! "height"(フィールド)
	JsonElement<uint16_t> height;
	//! "turn_team"(フィールド)
	JsonElement<std::string> turn_team;
	//! "players"(配列)
	JsonElement<std::vector<std::string>> players;
	//! "finished"(フィールド)
	JsonElement<bool> finished;
	//! "count"(フィールド)
	JsonElement<uint32_t> count;
	//! "units"(配列)
	JsonElement<std::vector<UnitsArrayElem>> units;
};

/**
*	@struct JSONRecvPacket_ConfirmName
*	@brief 「名前確定」パケット
*	@details 「名前確定」JSONの情報を格納するパケット
*/
struct JSONRecvPacket_ConfirmName {
	//! "your_team"(フィールド)
	JsonElement<std::string> your_team;
};

/**
*	@struct JSONRecvPacket_Result
*	@brief 「結果」パケット
*	@details 「結果」JSONの情報を格納するパケット
*/
struct JSONRecvPacket_Result {
	//! "result"(配列)
	JsonElement<std::vector<ResultArrayElem>> result;
};

/**
*	@struct JSONRecvPacket_Message
*	@brief 「メッセージ」パケット
*	@details 「メッセージ」JSONの情報を格納するパケット
*/
struct JSONRecvPacket_Message {
	//! "message"(フィールド)
	JsonElement<std::string> message;
};