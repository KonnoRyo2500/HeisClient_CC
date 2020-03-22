/**
*	@file		JSON_data_packet.h
*	@brief		heis JSONデータ管理用パケット
*	@author		Ryo Konno
*	@details	CCとサーバ間でやり取りされるJSONから得られる情報を取り扱うための構造体(パケット)を定義する．
*/
#pragma once

#include "heis_client_exception.h"
#include <string>
#include <vector>

/**
* @struct OptionalVal
* @brief 任意値パケット
*/
template <typename T>
struct OptionalVal {
	// メンバ関数
	public:
		//! コンストラクタ
		OptionalVal()
			: m_val()	// 警告抑止(この記述に特に意味はない)
			, m_omit_flag(true)
		{
			// 処理なし
		}

		//! 値の取得
		T get() const
		{
			if (m_omit_flag) {
				throw CHeisClientException("省略されたJSONキーを取得しようとしています");
			}
			return m_val;
		}

		//! 値が省略されているか
		bool is_omitted() const
		{
			return m_omit_flag;
		}

		/* 以下の関数は，CJSONAnalyzerクラスでパケットを作成するときだけ呼ぶこと */

		//! 値をセットする
		void set_val(const T& new_val)
		{
			m_val = new_val;
		}

		//! 省略フラグをセットする(値が省略されたとき)
		void set_omit_flag()
		{
			m_omit_flag = true;
		}

		//! 省略フラグを降ろす(値が省略されなかったとき)
		void clear_omit_flag()
		{
			m_omit_flag = false;
		}


	// メンバ変数
	private:
		//! 値本体
		T m_val;
		//! 省略フラグ(JSONで対応するキーが省略されていればtrue, あればfalse)
		bool m_omit_flag;
};

/* 以下のパケットの構造は，ファイル「兵ズ通信仕様.ods」で定義されているJSONの仕様に基づく */

/* 各種パケットの要素 */

/**
*	@struct	ContentsArrayElem
*	@brief 「行動」パケット -> contents(配列)の要素
*/
struct ContentsArrayElem {
	//! "unit_id"フィールドの値
	std::string unit_id;
	//! "to"."x"フィールドの値
	uint16_t to_x;
	//! "to"."y"フィールドの値
	uint16_t to_y;
	//! "atk"."x"フィールドの値
	uint16_t atk_x;
	//! "atk"."y"フィールドの値
	uint16_t atk_y;	
};
 
/**
*	@struct LocateObjData
*	@brief 「盤面」パケット -> units -> locate(オブジェクト)のデータ
*/
struct LocateObjData {
	//! "x"(フィールド)
	uint16_t x;
	//! "y"(フィールド)
	uint16_t y;
};

/**
*	@struct UnitsArrayElem
*	@brief 「盤面」パケット -> unitsの要素
*/
struct UnitsArrayElem {
	//! "type"(フィールド)
	std::string type;
	//! "unit_id"(フィールド)
	std::string unit_id;
	//! "locate"(オブジェクト)
	LocateObjData locate;
	//! "hp"(フィールド)
	int8_t hp;
	//! "team"(フィールド)
	std::string team;
};

/**
*	@struct ResultArrayElem
*	@brief 「結果」パケット -> resultの要素
*/
struct ResultArrayElem {
	//! "unit_id"(フィールド, 省略可能)
	OptionalVal<std::string> unit_id;
	//! "error"(フィールド)
	std::string error;
};

/* サーバに送信するJSONを作成するためのパケット */
 
/**
*	@struct JSONSendPacket_Name
*	@brief 「名前」パケット
*	@detail 「名前」JSONの情報を格納するパケット
*/
struct JSONSendPacket_Name {
	//! "team_name"(フィールド)
	std::string team_name;
};

/**
*	@struct JSONSendPacket_Action
*	@brief 「行動」パケット
*	@detail 「行動」JSONの情報を格納するパケット
*/
struct JSONSendPacket_Action {
	//! "turn_team"(フィールド)
	std::string turn_team;
	//! "contents"(配列)
	std::vector<ContentsArrayElem> contents;
};

/* サーバから受信したJSONのデータを格納するパケット */

// 「盤面」パケット
/**
*	@struct JSONRecvPacket_Field
*	@brief 「盤面」パケット
*	@detail 「盤面」JSONの情報を格納するパケット
*/
struct JSONRecvPacket_Field {
	//! "width"(フィールド)
	uint16_t width;
	//! "height"(フィールド)
	uint16_t height;
	//! "turn_team"(フィールド)
	std::string turn_team;
	//! "players"(配列)
	std::vector<std::string> players;
	//! "finished"(フィールド)
	bool finished;
	//! "count"(フィールド)
	uint32_t count;
	//! "units"(配列)
	std::vector<UnitsArrayElem> units;
};

/**
*	@struct JSONRecvPacket_NameDecided
*	@brief 「名前確定」パケット
*	@detail 「名前確定」JSONの情報を格納するパケット
*/
struct JSONRecvPacket_NameDecided {
	//! "your_team"(フィールド)
	std::string your_team;
};

/**
*	@struct JSONRecvPacket_Result
*	@brief 「結果」パケット
*	@detail 「結果」JSONの情報を格納するパケット
*/
struct JSONRecvPacket_Result {
	//! "result"(配列)
	std::vector<ResultArrayElem> result;
};

/**
*	@struct JSONRecvPacket_Message
*	@brief 「メッセージ」パケット
*	@detail 「メッセージ」JSONの情報を格納するパケット
*/
struct JSONRecvPacket_Message {
	//! "message"(フィールド)
	std::string message;
};