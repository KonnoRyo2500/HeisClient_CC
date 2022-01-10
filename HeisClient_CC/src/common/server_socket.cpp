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
{
	// 処理なし
}

/**
*	@brief デストラクタ
*/
CServerSocket::~CServerSocket()
{
	// 処理なし
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

	// 慣習に従い、listenの第二引数(backlog)は5にしておく
	ercd = listen(m_sck, 5);
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
	socklen_t addr_info_len = sizeof(sockaddr_in);
	
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

/* private関数 */