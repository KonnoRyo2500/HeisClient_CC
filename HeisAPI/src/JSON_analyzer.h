// heis JSON解析クラス
// Author: Ryo Konno
#pragma once

#include "socket.h"
#include "commander.h"
#include "game_online.h"
#include "field.h"
#include "picojson.h"

class CJSONAnalyzer{
	// 構造体，列挙体など
	public:
		// 受信したJSONの種別
		enum RecvJSONType {
			RecvJSONType_Field,				// 「盤面」JSON
			RecvJSONType_Result,			// 「結果」JSON
			RecvJSONType_NameDecided,		// 「名前確定」JSON
			RecvJSONType_Message,			// 「メッセージ」JSON
			RecvJSONType_UnknownJSON,		// 未定義のJSON
			RecvJSONType_NoJSONReceived,	// まだJSONを受信していない
		};

	private:

	// メンバ関数
	public:
		// コンストラクタ
		explicit CJSONAnalyzer();

		// デストラクタ
		~CJSONAnalyzer();

		// 必要な情報をサーバへ送信
		void send_action_data(const CCommander::JSONSendData_Action& action_data);
		void send_name_data(const CGameOnline::JSONSendData_Name& name_data);

		// サーバから情報を取得
		CGameOnline::JSONRecvData_NameDecided recv_name_decided_data();
		CGameOnline::JSONRecvData_Result recv_result_data();
		CField::JSONRecvData_Field recv_field_data();

		// 受信したJSONの種別を取得
		RecvJSONType get_received_JSON_type();

	private:
		// JSONの送受信
		void recv_JSON();
		void send_JSON(const std::string& JSON);

		// 受信したJSONの種類を判定
		RecvJSONType distinguish_recv_JSON_type();

	// メンバ変数
	private:
		// サーバとの通信用ソケット
		CSocket* m_sck;
		// 直前に受信したJSONの種類
		RecvJSONType m_latest_JSON_type;
		// 受信したパース済みのJSONデータ
		picojson::value m_parsed_recv_JSON;
};