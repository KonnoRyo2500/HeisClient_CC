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
{
	// 処理なし
}

/**
*	@brief デストラクタ
*/
CClientSocket::~CClientSocket()
{
	// 処理なし
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

/* private関数 */