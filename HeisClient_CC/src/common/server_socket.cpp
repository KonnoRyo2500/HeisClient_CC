/**
*	@file		server_socket.cpp
*	@brief		heis TCP/IP通信用ソケットクラス(サーバ用)
*	@author		Ryo Konno
*	@details	TCP/IPソケットの操作をより抽象化したインターフェイスを提供する(サーバ用)．
*/
#include "server_socket.h"
#include "common.h"

#include <stdexcept>
#ifdef WIN32
#include <WS2tcpip.h>
#else
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <cstring>
#endif // WIN32

/* public関数 */

/**
*	@brief コンストラクタ
*/
CServerSocket::CServerSocket()
	: m_sck(0)
{
	// Windows環境で動作させる場合，ソケット通信にwinsockを使うので，その初期化を行う(windows環境以外ならば何もしない)
	initialize_socket();
	sck_socket();
}

/**
*	@brief デストラクタ
*/
CServerSocket::~CServerSocket()
{
	finalize_socket();
}

/**
*	@brief サーバ自身のアドレス情報をソケットに紐づけする関数
*	@param[in] svr_port_no サーバのポート番号
*	@param[in] clt_ip_addr = "0.0.0.0" 接続を許可するクライアントのIPアドレス(未指定の場合は任意のIPアドレスからの接続を受け付ける)
*/
void CServerSocket::sck_bind(const uint16_t svr_port_no, const std::string& clt_ip_addr) const
{
	sockaddr_in addr = { 0 };
	int ercd;

	addr.sin_family = AF_INET;
	addr.sin_port = htons(svr_port_no);
	ercd = inet_pton(AF_INET, clt_ip_addr.c_str(), &addr.sin_addr);
	if (ercd <= 0) {
		throw std::runtime_error(cc_common::format("IPアドレス\"%s\"は不正です", clt_ip_addr.c_str()));
	}

	ercd = bind(m_sck, reinterpret_cast<sockaddr*>(&addr), sizeof(addr));
	if (ercd < 0) {
		throw std::runtime_error(cc_common::format("ソケットのバインドに失敗しました(エラーコード: %d)", errno));
	}
}

/**
*	@brief ソケットを接続待ち受け可能にする関数
*/
void CServerSocket::sck_listen() const
{
	int ercd;

	ercd = listen(m_sck, SocketConstVal_ConnectReqQueueLen);
	if (ercd < 0) {
		throw std::runtime_error(cc_common::format("接続待ちに失敗しました(エラーコード: %d)", errno));
	}
}

/**
*	@brief クライアントからの接続を待ち受ける関数
*	@remark クライアントと接続後に、この関数を再度呼んではいけない
*/
void CServerSocket::sck_accept()
{
	sockaddr_in client_addr_info = { 0 };
	int new_sck;
	int addr_info_len = sizeof(sockaddr_in);
	
	// 通信用ソケット作成
	new_sck = accept(m_sck, reinterpret_cast<sockaddr*>(&client_addr_info), &addr_info_len);
	if (new_sck < 0) {
		throw std::runtime_error(cc_common::format("クライアントとの接続に失敗しました(エラーコード: %d)", errno));
	}

	// クライアントとは1対1で接続するため、以降の接続受け付けは不要
	// そのため、元々接続受け付け用だったソケットはここで閉じる
	sck_close();

	// 以降、m_sckは通信用ソケットになる
	m_sck = new_sck;
}

/**
*	@brief クライアントにデータを送信する関数
*	@param[in] data 送信するデータ
*	@param[in] etx 終端文字(省略可。省略した場合は改行コード('\n' = 0x20))
*/
void CServerSocket::sck_send(const std::string& data, const char etx) const
{
	std::string send_data = data + etx;
	size_t sent_size = send(m_sck, send_data.c_str(), send_data.size(), 0);

	if (sent_size < send_data.size()) {
		if (sent_size < 0) {
			throw std::runtime_error(cc_common::format("送信でエラーが発生しました(エラーコード: %d)", errno));
}
		fprintf(stderr, "警告: 不完全なデータが送信されました(%zuバイト中%zuバイトが送信されました)\n", send_data.size(), sent_size);
	}
}

/**
*	@brief クライアントからデータを受信する関数
*	@param[in] etx 終端文字(省略可。省略した場合は改行コード('\n' = 0x20))
*	@return std::string 受信したデータ
*/
std::string CServerSocket::sck_recv(const char etx)
{
	// 前回の受信データに終端文字が含まれていれば、新たに受信は行わず即座に終端文字までのデータを返す
	if (cc_common::contains(m_prev_recv_remaind_data, etx)) {
		return cc_common::cut_string(m_prev_recv_remaind_data, 0, m_prev_recv_remaind_data.find(etx));
	}

	std::string recv_data = "";
	char buf[SocketConstVal_RecvBufSize];

	// データを受信
	do {
		memset(buf, 0, sizeof(buf));
		if (recv(m_sck, buf, sizeof(buf), 0) < 0) {
			throw std::runtime_error(cc_common::format("受信でエラーが発生しました(エラーコード: %d)", errno));
		}
		recv_data += buf;
	} while (!cc_common::contains(recv_data, etx));

	// 終端文字以降のデータは、次の受信時に読みだすようにする
	m_prev_recv_remaind_data = cc_common::cut_string(recv_data, recv_data.find_first_of(etx) + 1);

	return recv_data;
}

/* private関数 */

/**
*	@brief TCP通信用ソケットを作成する関数
*	@throws std::runtime_error ソケットの作成に失敗したとき
*/
void CServerSocket::sck_socket()
{
	m_sck = socket(AF_INET, SOCK_STREAM, 0);
	if (m_sck < 0) {
		throw std::runtime_error(cc_common::format("ソケットの作成に失敗しました(エラーコード: %d)", errno));
	}
}

/**
*	@brief ソケットを閉じる関数
*/
void CServerSocket::sck_close() const
{
#ifdef WIN32
	closesocket(m_sck);
#else
	close(m_sck);
#endif // WIN32
}

/**
*	@brief ソケットを初期化する関数
*	@throws std::runtime_error ソケットの初期化に失敗したとき
*/
void CServerSocket::initialize_socket() const
{
#ifdef WIN32
	WSADATA wsaData;
	int init_ercd;

	init_ercd = WSAStartup(MAKEWORD(2, 0), &wsaData);
	if (init_ercd != 0) {
		throw std::runtime_error(cc_common::format("winsockの初期化に失敗しました(エラーコード: %d)", init_ercd));
	}
#endif // WIN32
}

/**
*	@brief winsockの終了処理を行う関数
*/
void CServerSocket::finalize_socket() const
{
#ifdef WIN32
	WSACleanup();
#endif // WIN32
	sck_close();
}