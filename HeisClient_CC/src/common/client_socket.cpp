/**
*	@file		client_socket.cpp
*	@brief		TCP/IP通信用ソケットクラス(クライアント用)
*	@author		Ryo Konno
*	@details	TCP/IPソケットの操作をより抽象化したインターフェイスを提供する(クライアント用)．
*/
#include "client_socket.h"
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
CClientSocket::CClientSocket()
	: m_sck(0)
	, m_prev_recv_remaind_data("")
{
	initialize_socket();
	sck_socket();
}

/**
*	@brief デストラクタ
*/
CClientSocket::~CClientSocket()
{
	finalize_socket();
}

/**
*	@brief TCP通信用ソケットを相手に接続する関数
*	@param[in] dst_ip_addr 通信相手のIPアドレス
*	@param[in] dst_port_no 通信相手のポート番号
*/
void CClientSocket::sck_connect(const std::string& dst_ip_addr, const uint16_t dst_port_no) const
{
	sockaddr_in sa = { 0 };
	int ercd;

	sa.sin_family = AF_INET;
	sa.sin_port = htons(dst_port_no);
	ercd = inet_pton(AF_INET, dst_ip_addr.c_str(), &sa.sin_addr);
	if (ercd <= 0) {
		throw std::runtime_error(cc_common::format("IPアドレス\"%s\"は不正です", dst_ip_addr.c_str()));
	}

	ercd = connect(m_sck, reinterpret_cast<sockaddr*>(&sa), sizeof(sa));
	if (ercd < 0) {
		throw std::runtime_error(cc_common::format("サーバとの接続に失敗しました(エラーコード: %d)", errno));
	}
}

/**
*	@brief データを送信する関数
*	@param[in] data 送信するデータ
*	@param[in] etx 終端文字(省略可。省略した場合は改行コード('\n' = 0x20))
*	@throws std::runtime_error 送信エラーが発生したとき
*/
void CClientSocket::sck_send(const std::string& data, const char etx) const
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
*	@brief サーバからデータを受信する関数
*	@param[in] etx 終端文字(省略可。省略した場合は改行コード('\n' = 0x20))
*	@return std::string サーバから受信したデータ
*/
std::string CClientSocket::sck_recv(const char etx)
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
*	@brief ソケットを初期化する関数
*	@throws std::runtime_error ソケットの初期化に失敗したとき
*/
void CClientSocket::initialize_socket() const
{
#ifdef WIN32
	WSADATA wsaData;
	int init_ercd;

	init_ercd = WSAStartup(MAKEWORD(2, 0), &wsaData);
	if (init_ercd != 0) {
		// winsockの初期化失敗
		// 初期化に失敗すると，その後の処理が不安定になるので失敗したら例外を投げてプログラムを終了させる
		throw std::runtime_error(cc_common::format("winsockの初期化に失敗しました(エラーコード: %d)", init_ercd));
	}
#endif // WIN32
}

/**
*	@brief winsockの終了処理を行う関数
*/
void CClientSocket::finalize_socket() const
{
#ifdef WIN32
	WSACleanup();
	closesocket(m_sck);
#else
	close(m_sck);
#endif // WIN32
}

/**
*	@brief TCP通信用ソケットを作成する関数
*	@throws std::runtime_error ソケットの作成に失敗したとき
*/
void CClientSocket::sck_socket()
{
	m_sck = socket(AF_INET, SOCK_STREAM, 0);
	if (m_sck < 0) {
		throw std::runtime_error(cc_common::format("ソケットの作成に失敗しました(エラーコード: %d)", errno));
	}
}