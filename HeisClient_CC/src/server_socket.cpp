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
	, m_dest_info()
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
	引数2: const std::string& clt_ip_addr& = "0.0.0.0" 接続を許可するクライアントのIPアドレス(未指定の場合は任意のIPアドレスからの接続を受け付ける)
	返り値なし
*/
void CServerSocket::sck_bind(const uint16_t svr_port_no, const std::string& clt_ip_addr) const
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

	// クライアントの情報と，そのクライアントと通信するソケットを紐づける
	m_dest_info[std::make_pair(std::string(client_ip_addr), client_port_no)] = new_sck;
}

/*
	クライアントにメッセージを送信する関数
	引数1: const std::string& msg 送信するメッセージ
	引数2: const std::string& clt_ip_addr = "" 送信先のIPアドレス(指定がない場合は最初に接続したクライアントのIPアドレス)
	引数3: const uint16_t clt_port_no = 0 送信先のポート番号(指定がない場合は最初に接続したクライアントのポート番号)
*/
void CServerSocket::sck_sendto(const std::string& msg, const std::string& clt_ip_addr, const uint16_t clt_port_no) const
{
	int com_sck = client_info_to_socket(clt_ip_addr, clt_port_no);

	// 通信用ソケットを決定
	if (com_sck < 0) {
		if (clt_ip_addr.size() <= 0) {
			com_sck = m_dest_info.begin()->second;
		}
		else {
			throw CHeisClientException("指定されたクライアント(IP: %s, ポート番号: %d)は未接続です．", clt_ip_addr.c_str(), clt_port_no);
		}
	}

	// メッセージを送信
	// メッセージを確実にNULL終端させるため，size + 1文字送信する
	size_t send_size = send(com_sck, msg.c_str(), msg.size() + 1, 0);
	if (send_size < msg.size()) {
		if (send_size < 0) {
			throw CHeisClientException("送信でエラーが発生しました(エラーコード: %d)", errno);
		}
		fprintf(stderr, "警告: 不完全なメッセージが送信されました(%d文字中%d文字が送信されました)\n", msg.size(), send_size);
	}
}

/*
	クライアントからメッセージを受信する関数
	引数1: const std::string& clt_ip_addr = "" 受信元のIPアドレス(指定がない場合は最初に接続したクライアントのIPアドレス)
	引数2: const uint16_t clt_port_no = 0 受信元のポート番号(指定がない場合は最初に接続したクライアントのポート番号)
*/
std::string CServerSocket::sck_recvfrom(const std::string& clt_ip_addr, const uint16_t clt_port_no) const
{
	return "";
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
	for (auto& it : m_dest_info) {
		closesocket(it.second);
	}
#else
	close(m_sck_accept);
	for (auto& it : m_dest_info) {
		close(it.second);
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

/*
	クライアントのIPアドレスとポート番号から，そのクライアントと通信するためのソケットを取得する関数
	引数1: const std::string& clt_ip_addr クライアントのIPアドレス
	引数2: const uint16_t clt_port_no クライアントのポート番号
	返り値: int クライアントとの通信用ソケット(IPとポート番号に対応するソケットが見つからなければ-1)
*/
int CServerSocket::client_info_to_socket(const std::string& clt_ip_addr, const uint16_t clt_port_no) const
{
	int com_sck = -1;
	auto it = m_dest_info.find(std::make_pair(clt_ip_addr, clt_port_no));

	if (it != m_dest_info.end()) {
		com_sck = it->second;
	}

	return com_sck;
}