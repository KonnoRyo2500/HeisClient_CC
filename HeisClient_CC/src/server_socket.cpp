// heis TCP/IP通信用ソケットクラス(サーバ用)
// Author: Ryo Konno

#include "server_socket.h"
#include "heis_client_exception.h"

#ifdef WIN32
#include <WS2tcpip.h>
#else
#endif
/* public関数 */

/*
	コンストラクタ
	引数なし
*/
CServerSocket::CServerSocket()
	: m_sck_accept(0)
	, m_sck_com()
{
	// Windows環境で動作させる場合，ソケット通信にwinsockを使うので，その初期化を行う(windows環境以外ならば何もしない)
	initialize_socket();
	sck_socket();
}

/*
	デストラクタ
	引数なし
*/
CServerSocket::~CServerSocket()
{
	finalize_socket();
}

/*
	サーバ自身のアドレス情報をソケットに紐づけする関数
	引数1: const uint16_t svr_port_no サーバのポート番号
	引数2: const std::string clt_ip_addr = "0.0.0.0" 接続を許可するクライアントのIPアドレス(未指定の場合は任意のIPアドレスからの接続を受け付ける)
	返り値なし
*/
void CServerSocket::sck_bind(const uint16_t svr_port_no, const std::string clt_ip_addr) const
{
	sockaddr_in addr = { 0 };
	int ercd;

	addr.sin_family = AF_INET;
	addr.sin_port = htons(svr_port_no);
	ercd = inet_pton(AF_INET, clt_ip_addr.c_str(), &addr.sin_addr);
	if (ercd <= 0) {
		throw CHeisClientException("IPアドレス\"%s\"は不正です", clt_ip_addr.c_str());
	}

	ercd = bind(m_sck_accept, reinterpret_cast<sockaddr*>(&addr), sizeof(addr));
	if (ercd < 0) {
		throw CHeisClientException("ソケットのバインドに失敗しました(エラーコード: %d)", errno);
	}
}

/*
	ソケットを接続待ち受け可能にする関数
	引数なし
	返り値なし
*/
void CServerSocket::sck_listen() const
{
	int ercd;

	ercd = listen(m_sck_accept, SocketConstVal_ConnectReqQueueLen);
	if (ercd < 0) {
		throw CHeisClientException("接続待ちに失敗しました(エラーコード: %d)", errno);
	}
}

/*
	クライアントからの接続を待ち受ける関数
	引数なし
	返り値なし
*/
void CServerSocket::sck_accept()
{
	sockaddr_in client_addr_info = { 0 };
	int new_sck;
	int addr_info_len = sizeof(sockaddr_in);
	char client_ip_addr[INET_ADDRSTRLEN] = { 0 };
	uint16_t client_port_no;
	
	// 通信用ソケット作成
	new_sck = accept(m_sck_accept, reinterpret_cast<sockaddr*>(&client_addr_info), &addr_info_len);
	if (new_sck < 0) {
		throw CHeisClientException("クライアントとの接続に失敗しました(エラーコード: %d)", errno);
	}

	// クライアントの情報を取得
	inet_ntop(AF_INET, &client_addr_info.sin_addr, client_ip_addr, INET_ADDRSTRLEN);
	client_port_no = client_addr_info.sin_port;

	m_client_info.push_back(std::make_pair(std::string(client_ip_addr), client_port_no));
	m_sck_com.push_back(new_sck);
}

/* private関数 */

/*
	TCP通信用ソケットを作成する関数
	引数なし
	返り値なし
	例外: ソケットの作成に失敗したとき
*/
void CServerSocket::sck_socket()
{
	m_sck_accept = socket(AF_INET, SOCK_STREAM, 0);
	if (m_sck_accept < 0) {
		throw CHeisClientException("ソケットの作成に失敗しました(エラーコード: %d)", errno);
	}
}

/*
	メンバ変数として保持しているすべてのソケットを閉じる関数
	引数なし
	返り値なし
*/
void CServerSocket::sck_close() const
{
#ifdef WIN32
	closesocket(m_sck_accept);
	for (auto& sck_com : m_sck_com) {
		closesocket(sck_com);
	}
#else
	close(m_sck_accept);
	for (auto& sck_com : m_sck_com) {
		close(sck_com);
	}
#endif // WIN32
}

/*
	ソケットを初期化する関数
	引数なし
	返り値なし
	例外: ソケットの初期化に失敗したとき
*/
void CServerSocket::initialize_socket() const
{
#ifdef WIN32
	WSADATA wsaData;
	int init_ercd;

	init_ercd = WSAStartup(MAKEWORD(2, 0), &wsaData);
	if (init_ercd != 0) {
		// winsockの初期化失敗
		// 初期化に失敗すると，その後の処理が不安定になるので失敗したら例外を投げてプログラムを終了させる
		throw CHeisClientException("winsockの初期化に失敗しました(エラーコード: %d)", init_ercd);
	}
#endif // WIN32
}

/*
	winsockの終了処理を行う関数
	引数なし
	返り値なし
*/
void CServerSocket::finalize_socket() const
{
#ifdef WIN32
	WSACleanup();
#endif // WIN32
	sck_close();
}