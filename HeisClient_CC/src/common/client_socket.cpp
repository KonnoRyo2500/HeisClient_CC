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
*	@throws std::runtime_error 送信エラーが発生したとき
*/
void CClientSocket::sck_send(const std::string& data) const
{
	// サーバが前回送信されたデータを受信中に再度送信することを防ぐため，少しだけ待ちを入れる
#ifdef WIN32
	Sleep(SocketConstVal_SendIntervalTimeMs);
#else
	usleep(SocketConstVal_SendIntervalTimeMs * 1000);
#endif

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
*	@brief サーバからデータを受信する関数
*	@return std::string サーバから受信したデータ
*/
std::string CClientSocket::sck_recv() const
{
#ifdef WIN32
	return sck_recv_core_win();
#else
	return sck_recv_core_linux();
#endif
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

// 受信処理に関しては、#ifdefが関数中に入り乱れるのを防ぐため、プラットフォーム別に関数を分ける
// TODO: プラットフォームに依らない受信処理の実装
/**
*	@brief 受信処理(Windows向け)
*	@return std::string 受信したデータ
*	@throws std::runtime_error 受信エラーが発生したとき
*/
std::string CClientSocket::sck_recv_core_win() const
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
			if (WSAGetLastError() != WSAEWOULDBLOCK) {
				throw std::runtime_error(cc_common::format("受信でエラーが発生しました(エラーコード: %d)", WSAGetLastError()));
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
std::string CClientSocket::sck_recv_core_linux() const
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
