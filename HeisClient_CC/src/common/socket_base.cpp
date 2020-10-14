/**
*	@file		socket_base.cpp
*	@brief		TCP/IP通信用ソケット基本クラス
*	@author		Ryo Konno
*	@details	各種TCP/IPソケットクラスの基本となるクラス。
*/

#include "socket_base.h"
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
CSocketBase::CSocketBase()
{
#ifdef WIN32
	initialize_winsock();
#endif // WIN32
	sck_socket();
}

/**
*	@brief デストラクタ
*/
CSocketBase::~CSocketBase()
{
#ifdef WIN32
	WSACleanup();
#endif // WIN32
}

/**
*	@brief データを送信する関数
*	@param[in] data 送信するデータ
*	@param[in] etx 終端文字(省略可。省略した場合は改行コード('\n' = 0x20))
*/
void CSocketBase::sck_send(const std::string& data, const char etx) const
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
*	@brief データを受信する関数
*	@param[in] etx 終端文字(省略可。省略した場合は改行コード('\n' = 0x20))
*	@return std::string 受信したデータ
*/
std::string CSocketBase::sck_recv(const char etx)
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

/* protected関数 */

/**
* @brief winsockを初期化する関数
* @throws std::runtime_error winsockの初期化に失敗したとき
*/
void CSocketBase::initialize_winsock() const
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
*	@brief ソケットを閉じる関数
*/
void CSocketBase::sck_close() const
{
#ifdef WIN32
	closesocket(m_sck);
#else
	close(m_sck);
#endif // WIN32
}

/**
*	@brief TCP通信用ソケットを作成する関数
*	@throws std::runtime_error ソケットの作成に失敗したとき
*/
void CSocketBase::sck_socket()
{
	m_sck = socket(AF_INET, SOCK_STREAM, 0);
	if (m_sck < 0) {
		throw std::runtime_error(cc_common::format("ソケットの作成に失敗しました(エラーコード: %d)", errno));
	}
}