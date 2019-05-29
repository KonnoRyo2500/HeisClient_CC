// heis TCP/IP通信用ソケットクラス
// Author: Ryo Konno

#include "socket.h"
#ifdef WIN32
#include <winsock2.h>
#endif // WIN32
#include <stdexcept>


/* public関数 */

/*
	コンストラクタ
	引数なし
*/
CSocket::CSocket()
{
	// Windows環境で動作させる場合，ソケット通信にwinsockを使うので，その初期化を行う(windows環境以外ならば何もしない)
	initialize_winsock();
}

/*
	デストラクタ
	引数なし
*/
CSocket::~CSocket()
{
	// Windows環境で動作させる場合，ソケット通信にwinsockを使うので，その終了処理を行う
	finalize_winsock();
}

/* private関数 */

/*
	winsockを初期化する関数
	引数なし
	返り値なし
	備考: この関数は，windows環境以外の環境では何もしない
*/
void CSocket::initialize_winsock()
{
#ifdef WIN32
	WSADATA wsaData;
	int init_ercd;

	init_ercd = WSAStartup(MAKEWORD(2, 0), &wsaData);
	if (init_ercd != 0) {
		// winsockの初期化失敗
		// 初期化に失敗すると，その後の処理が不安定になるので失敗したら例外を投げてプログラムを終了させる
		throw std::runtime_error("winsockの初期化に失敗しました");
	}
#endif // WIN32
}

/*
	winsockの終了処理を行う関数
	引数なし
	返り値なし
	備考: この関数は，windows環境以外の環境では何もしない
*/
void CSocket::finalize_winsock()
{
#ifdef WIN32
	WSACleanup();
#endif // WIN32
}