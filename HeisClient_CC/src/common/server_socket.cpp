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
*/
void CServerSocket::sck_send(const std::string& data) const
{
	// サーバが前回送信されたデータを受信中に再度送信することを防ぐため，少しだけ待ちを入れる
#ifdef WIN32
	Sleep(SocketConstVal_SendIntervalTimeMs);
#else
	usleep(SocketConstVal_SendIntervalTimeMs * 1000);
#endif // WIN32

	// データを送信
	// データを確実にNULL終端させるため，size + 1バイト送信する
	size_t send_size = send(m_sck, data.c_str(), data.size() + 1, 0);
	if (send_size < data.size()) {
		if (send_size < 0) {
			throw std::runtime_error(cc_common::format("送信でエラーが発生しました(エラーコード: %d)", errno));
		}
		fprintf(stderr, "警告: 不完全なデータが送信されました(%zuバイト中%zuバイトが送信されました)\n", data.size(), send_size);
	}
}

/**
*	@brief クライアントからデータを受信する関数
*	@return std::string 受信したデータ
*/
std::string CServerSocket::sck_recv() const
{
	// データを受信
#ifdef WIN32
	return sck_recv_core_win();
#else
	return sck_recv_core_linux();
#endif
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

// 受信処理に関しては、#ifdefが関数中に入り乱れるのを防ぐため、プラットフォーム別に関数を分ける
// TODO: プラットフォームに依らない受信処理の実装
/**
*	@brief 受信処理(Windows向け)
*	@return std::string 受信したデータ
*	@throws std::runtime_error 受信エラーが発生したとき
*/
std::string CServerSocket::sck_recv_core_win() const
{
#ifdef WIN32
	// データを確実にNULL終端させるため，バッファは1バイト余分に取る
	char buf[SocketConstVal_RecvBufSize + 1] = { 0 };
	int recv_size;
	std::string recv_data;

	// データの到着前に抜けてしまうのを防ぐため，最初の受信はブロッキングにする
	recv_size = recv(m_sck, buf, sizeof(buf) - 1, 0);
	if (recv_size < 0) {
		throw std::runtime_error(cc_common::format("受信でエラーが発生しました(エラーコード: %d)", errno));
	}
	recv_data += std::string(buf);

	// 入力キューにデータが残っていれば，それらもすべて受信する
	{
		// 受信データがないときに無限待ちにならないよう，一旦ソケットをノンブロッキングにする
		unsigned long nonblocking_enable = 1;
		ioctlsocket(m_sck, FIONBIO, &nonblocking_enable);
	}
	while (recv_size == sizeof(buf) - 1) {
		memset(buf, 0, sizeof(buf));
		recv_size = recv(m_sck, buf, sizeof(buf) - 1, 0);
		if (recv_size < 0) {
			if (WSAGetLastError() == WSAEWOULDBLOCK) {
				throw std::runtime_error(cc_common::format("受信でエラーが発生しました(エラーコード: %d)", errno));
			}
			else {
				break;
			}
		}
		recv_data += std::string(buf);
	}
	// 次の呼び出しでの最初の受信をブロッキングにするため，ソケットをブロッキングに戻す
	{
		unsigned long nonblocking_disable = 0;
		ioctlsocket(m_sck, FIONBIO, &nonblocking_disable);
	}

	return recv_data;
#else
	return "";
#endif // WIN32
}

/**
*	@brief 受信処理(Linux向け)
*	@return std::string 受信したデータ
*	@throws std::runtime_error 受信エラーが発生したとき
*/
std::string CServerSocket::sck_recv_core_linux() const
{
#ifndef WIN32
	// データを確実にNULL終端させるため，バッファは1バイト余分に取る
	char buf[SocketConstVal_RecvBufSize + 1] = { 0 };
	int recv_size;
	std::string recv_data;

	// データの到着前に抜けてしまうのを防ぐため，最初の受信はブロッキングにする
	recv_size = recv(m_sck, buf, sizeof(buf) - 1, 0);
	if (recv_size < 0) {
		throw std::runtime_error(cc_common::format("受信でエラーが発生しました(エラーコード: %d)", errno));
	}
	recv_data += std::string(buf);

	// 入力キューにデータが残っていれば，それらもすべて受信する
	while (recv_size == sizeof(buf) - 1) {
		memset(buf, 0, sizeof(buf));
		// 受信データがないときに無限待ちにならないよう，ノンブロッキングで受信する
		recv_size = recv(m_sck, buf, sizeof(buf) - 1, MSG_DONTWAIT);
		if (recv_size < 0) {
			if (errno != EAGAIN) {
				throw std::runtime_error(cc_common::format("受信でエラーが発生しました(エラーコード: %d)", errno));
			}
			else {
				break;
			}
		}
		recv_data += std::string(buf);
	};

	return recv_data;
#else
	return "";
#endif // WIN32
}