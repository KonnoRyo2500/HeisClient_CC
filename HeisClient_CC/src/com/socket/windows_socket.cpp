/**
*	@file		windows_socket.cpp
*	@brief		Windows用TCP/IPソケットクラス
*	@author		Ryo Konno
*	@details	Windows環境におけるソケットの各種操作を提供する。
*/

#ifdef WIN32

#include "windows_socket.h"

#include <WS2tcpip.h>
#include <stdexcept>

/**
*	@def RECV_BUF_SIZE
*	@brief 受信バッファのサイズ
*/
#define RECV_BUF_SIZE (1000)

/**
*	@brief コンストラクタ
*	@details 初期化処理とsocketの呼び出しも同時に行う
*/
CWindowsSocket::CWindowsSocket()
{
	bool initialize_result = initialize();
	if (!initialize_result) {
		throw std::runtime_error("ソケットの初期化処理が失敗しました");
	}

	m_socket = wrap_socket();
	if (m_socket < 0) {
		throw std::runtime_error("ソケットの生成に失敗しました");
	}
}

/**
*	@brief ソケットに名前を付ける
*	@param[in] dst_port 接続先ポート番号
*	@param[in] src_addr 接続元IPアドレス
*/
void CWindowsSocket::wrap_bind(const uint16_t dst_port, const std::string& src_addr)
{
	sockaddr_in addr = { 0 };
	int ercd;

	addr.sin_family = AF_INET;
	addr.sin_port = htons(dst_port);
	ercd = inet_pton(AF_INET, src_addr.c_str(), &addr.sin_addr);
	if (ercd <= 0) {
		throw std::runtime_error("指定されたIPアドレスは不正です");
	}

	ercd = bind(m_socket, reinterpret_cast<sockaddr*>(&addr), sizeof(addr));
	if (ercd < 0) {
		throw std::runtime_error("bindシステムコールの呼び出しに失敗しました");
	}
}

/**
*	@brief 相手からの接続を受け付ける
*/
void CWindowsSocket::wrap_listen()
{
	// 慣習に従い、listenの第二引数(backlog)は5にしておく
	int ercd = listen(m_socket, 5);
	if (ercd < 0) {
		throw std::runtime_error("listenシステムコールの呼び出しに失敗しました");
	}
}

/**
*	@brief 相手からの接続を待つ
*/
void CWindowsSocket::wrap_accept()
{
	sockaddr_in client_addr_info = { 0 };
	socklen_t addr_info_len = sizeof(sockaddr_in);

	// 通信用ソケット作成
	int new_sck = accept(m_socket, reinterpret_cast<sockaddr*>(&client_addr_info), &addr_info_len);
	if (new_sck < 0) {
		throw std::runtime_error("acceptシステムコールの呼び出しに失敗しました");
	}

	// クライアントとは1対1で接続するため、以降の接続受け付けは不要
	// そのため、元々接続受け付け用だったソケットはここで閉じる
	wrap_close();

	// 以降、m_socketは通信用ソケットになる
	m_socket = new_sck;
}

/**
*	@brief ソケットを接続する
*	@param[in] addr 接続先IPアドレス
*	@param[in] port 接続先ポート番号
*/
void CWindowsSocket::wrap_connect(const std::string& addr, const uint16_t port) const
{
	sockaddr_in sa = { 0 };
	int ercd;

	sa.sin_family = AF_INET;
	sa.sin_port = htons(port);
	ercd = inet_pton(AF_INET, addr.c_str(), &sa.sin_addr);
	if (ercd <= 0) {
		throw std::runtime_error("inet_ptonシステムコールの呼び出しに失敗しました");
	}

	ercd = connect(m_socket, reinterpret_cast<sockaddr*>(&sa), sizeof(sa));
	if (ercd < 0) {
		throw std::runtime_error("connectシステムコールの呼び出しに失敗しました");
	}
}

/**
*	@brief データを送信する
*	@param[in] data 送信するデータ
*	@param[in] terminal 終端文字(省略時は付与しない)
*/
void CWindowsSocket::wrap_send(const std::string& data, const char terminal)
{
	std::string send_data = data + terminal;
	size_t sent_size = send(m_socket, send_data.c_str(), send_data.size(), 0);

	if (sent_size < send_data.size()) {
		if (sent_size < 0) {
			throw std::runtime_error("sendシステムコールの呼び出しに失敗しました");
		}
		fprintf(stderr, "警告: 不完全なデータが送信されました(%zuバイト中%zuバイトが送信されました)\n", send_data.size(), sent_size);
	}
}

/**
*	@brief データを受信する
*	@param[in] terminal 終端文字(省略時は付与しない)
*/
std::string CWindowsSocket::wrap_recv(const char terminal)
{
	std::string recv_data = "";

	while(true) {
		// ソケットの内部バッファをPEEKする
		char peek_buf[RECV_BUF_SIZE] = { 0 };
		recv(m_socket, peek_buf, sizeof(peek_buf) - 1, MSG_PEEK);

		// PEEKしたバッファから終端文字を探す
		int terminal_idx = -1;
		for (int i = 0; i < RECV_BUF_SIZE - 1; i++) {
			if (peek_buf[i] == terminal) {
				terminal_idx = i;
				break;
			}
		}
		
		// 終端文字があれば、その位置までデータを受信して処理終了
		// 無ければ、bufの最大サイズ分だけデータを受信し、再度PEEK + 受信する
		char buf[RECV_BUF_SIZE] = { 0 };
		if (terminal_idx != -1) {
			recv(m_socket, buf, terminal_idx + 1, 0);
			recv_data += buf;
			break;
		}
		else {
			recv(m_socket, buf, sizeof(buf) - 1, 0);
			recv_data += buf;
		}
	}

	return recv_data;
}

/**
*	@brief ソケットを閉じる
*/
void CWindowsSocket::wrap_close()
{
	closesocket(m_socket);
}

/**
*	@brief ソケットを作成する
*	@details TCP/IP通信用のソケットを作成する
*/
int CWindowsSocket::wrap_socket()
{
	return socket(AF_INET, SOCK_STREAM, 0);
}

/**
*	@brief 初期化処理を行う
*	@return bool 初期化処理が成功したか
*/
bool CWindowsSocket::initialize()
{
	WSADATA wsaData;
	int startup_result = WSAStartup(MAKEWORD(2, 0), &wsaData);
	return (startup_result == 0);
}

/**
*	@brief 終了処理を行う
*	@return bool 終了処理が成功したか
*/
bool CWindowsSocket::finalize()
{
	int cleanup_result = WSACleanup();
	return (cleanup_result >= 0);
}

#endif // WIN32