// heisクライアント用JSON送受信治具 送信クラス
// Author: Ryo Konno

#include <fstream>

#include "JSON_sender.h"
#include "heis_client_exception.h"

/* public関数 */

/*
	与えられたJSONを送信する関数
	引数1: const CServerSocket& sck 通信用ソケット
	引数2: const std::string& JSON 送信するJSON
	返り値なし
*/
void CJsonSender::send_JSON(const CServerSocket& sck, const std::string& JSON) const
{
	sck.sck_sendto(JSON);
}

/*
	JSONをファイルから読み込み，送信する関数
	引数1: const CServerSocket& sck 通信用ソケット
	引数2: const std::string& JSON_file_name JSONが記載されたファイル名
	返り値なし
*/
void CJsonSender::send_JSON_from_file(const CServerSocket& sck, const std::string& JSON_file_name) const
{
	std::ifstream JSON_file(JSON_file_name);
	std::string JSON_buff;
	std::string JSON;

	if(JSON_file.fail()){
		throw CHeisClientException("JSONファイルのオープンに失敗しました(ファイル名: %s)", JSON_file_name.c_str());
	}
	while(std::getline(JSON_file, JSON_buff)){
		JSON += JSON_buff;
	}
	sck.sck_sendto(JSON);
}

/* private関数 */
