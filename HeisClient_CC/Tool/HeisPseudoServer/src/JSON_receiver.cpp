// heisクライアント用JSON送受信治具 受信クラス
// Author: Ryo Konno

#include <fstream>

#include "JSON_receiver.h"
#include "heis_client_exception.h"

/* public関数 */

/*
	JSONを受信し，標準出力に出力する関数
	引数1: const CServerSocket& sck 通信用ソケット
	返り値なし
*/
void CJsonReceiver::recv_JSON_and_print(const CServerSocket& sck) const
{
	printf("%s\n", sck.sck_recvfrom().c_str());
}

/*
	JSONを受信し，指定されたファイルに書き込む関数
	引数1: const CServerSocket& sck 通信用ソケット
	引数2: const std::string& JSON_file_name 書き出し先のファイル名
	返り値なし
*/
void CJsonReceiver::recv_JSON_and_write_file(const CServerSocket& sck, const std::string& JSON_file_name) const
{
	std::ofstream JSON_file(JSON_file_name);

	if(JSON_file.fail()){
		throw CHeisClientException("JSONを書き出すファイルのオープンに失敗しました(ファイル名: %s)", JSON_file_name.c_str());
	}
	JSON_file << sck.sck_recvfrom() << std::endl;
}

/* private関数 */
