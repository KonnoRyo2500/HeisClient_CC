// heis JSON解析クラス
// Author: Ryo Konno

#include "JSON_analyzer.h"

/* public関数 */

/*
	コンストラクタ
	引数なし
*/
CJSONAnalyzer::CJSONAnalyzer()
	: m_sck(new CSocket("192.168.1.102", 8824))
	, m_latest_JSON_type(RecvJSONType_NoJSONReceived)
{
	// TODO: サーバのIPアドレスとポート番号を設定ファイルから取得するようにする
}

/*
	デストラクタ
	引数なし
*/
CJSONAnalyzer::~CJSONAnalyzer()
{
	delete m_sck;
	m_sck = NULL;
}

/*
	最後に受信したJSONの種類を取得する関数
	引数なし
	返り値: CJSONAnalyzer::RecvJSONType 最後に受信したJSONの種類
*/
CJSONAnalyzer::RecvJSONType CJSONAnalyzer::get_received_JSON_type()
{
	return m_latest_JSON_type;
}

/* private関数 */

/*
	JSONをサーバから受信する関数
	引数なし
	返り値なし
*/
void CJSONAnalyzer::recv_JSON()
{
	// JSONの受信+パース
	{
		std::string received_JSON = m_sck->recv_from();
		std::string errmsg = picojson::parse(m_parsed_recv_JSON, received_JSON);
		if (errmsg.size() > 0) {
			fprintf(stderr, "JSONのパースでエラーが発生しました(エラー内容: %s)\n", errmsg.c_str());
			throw std::runtime_error("JSONのパースに失敗しました");
		}
	}
	// 受信したJSONの種類を判定
	m_latest_JSON_type = distinguish_recv_JSON_type();
}

/*
	JSONをサーバに送信する関数
	引数: const std::string& JSON 送信するJSON
	返り値なし
*/
void CJSONAnalyzer::send_JSON(const std::string& JSON)
{
	m_sck->send_to(JSON);
}

/*
	受信したJSONの種類を判定する関数
	引数なし
	返り値: CJSONAnalyzer::RecvJSONType 受信したJSONの種類
*/
CJSONAnalyzer::RecvJSONType CJSONAnalyzer::distinguish_recv_JSON_type()
{
	return RecvJSONType_Message;
}