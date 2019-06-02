﻿// heis TCP/IP通信用ソケットクラス
// Author: Ryo Konno

#include "socket.h"
#ifdef WIN32
#include <winsock2.h>
#include <WS2tcpip.h>
#endif // WIN32
#include <stdexcept>

/* public関数 */

/*
	コンストラクタ
	引数1: const std::string& dst_ip_addr 通信相手のIPアドレス
	引数2: const uint16_t dst_port_no 通信相手のポート番号
*/
CSocket::CSocket(const std::string& dst_ip_addr, const uint16_t dst_port_no)
	: m_sck(0)
{
	// Windows環境で動作させる場合，ソケット通信にwinsockを使うので，その初期化を行う(windows環境以外ならば何もしない)
	initialize_winsock();
	make_TCP_socket();
	connect_TCP_socket(dst_ip_addr, dst_port_no);
}

/*
	デストラクタ
	引数なし
*/
CSocket::~CSocket()
{
	// Windows環境で動作させる場合，ソケット通信にwinsockを使うので，その終了処理を行う
	finalize_winsock();
}

/*
	メッセージを送信する関数
	引数1: const std::string& msg 送信するメッセージ
	返り値なし
	例外: 送信エラーが発生したとき
*/
void CSocket::send_to(const std::string& msg) const
{
	// メッセージを確実にNULL終端させるため，size + 1文字送信する
	int send_size = send(m_sck, msg.c_str(), msg.size() + 1, 0);

	if (static_cast<size_t>(send_size) < msg.size()) {
		if (send_size < 0) {
			throw std::runtime_error("送信でエラーが発生しました");
		}
		fprintf(stderr, "警告: 不完全なメッセージが送信されました\n");
	}
}

/*
	サーバからメッセージを受信する関数
	引数なし
	返り値: std::string サーバから受信したメッセージ
	例外: 受信エラーが発生したとき
*/
std::string CSocket::recv_from() const
{
	// メッセージを確実にNULL終端させるため，バッファは1バイト余分に取る
	char buf[SocketConstVal_RecvBufSize + 1] = { 0 };
	int recv_size;
	std::string recv_message;

	// データの到着前に抜けてしまうのを防ぐため，最初の受信はブロッキングにする
	recv_size = recv(m_sck, buf, sizeof(buf) - 1, 0);
	if (recv_size < 0) {
		throw std::runtime_error("受信でエラーが発生しました");
	}
	recv_message += std::string(buf);

	// 入力キューにデータが残っていれば，それらもすべて受信する
	{
		unsigned long nonblocking_enable = 1;
		ioctlsocket(m_sck, FIONBIO, &nonblocking_enable);
	}
	do {
		memset(buf, 0, sizeof(buf));
		recv_size = recv(m_sck, buf, sizeof(buf) - 1, 0);
		if (recv_size < 0) {
			if (WSAGetLastError() == WSAEWOULDBLOCK) {
				break;
			}
			else {
				throw std::runtime_error("受信でエラーが発生しました");
			}
		}
		recv_message += std::string(buf);
	} while (recv_size > 0);

	return recv_message;
}

/* private関数 */

/*
	winsockを初期化する関数
	引数なし
	返り値なし
	例外: winsockの初期化に失敗したとき
	備考: この関数は，windows環境以外の環境では何もしない
*/
void CSocket::initialize_winsock() const
{
#ifdef WIN32
	WSADATA wsaData;
	int init_ercd;

	init_ercd = WSAStartup(MAKEWORD(2, 0), &wsaData);
	if (init_ercd != 0) {
		// winsockの初期化失敗
		// 初期化に失敗すると，その後の処理が不安定になるので失敗したら例外を投げてプログラムを終了させる
		throw std::runtime_error("winsockの初期化に失敗しました");
	}
#endif // WIN32
}

/*
	TCP通信用ソケットを作成する関数
	引数なし
	返り値なし
	例外: ソケットの作成に失敗したとき
*/
void CSocket::make_TCP_socket()
{
	m_sck = socket(AF_INET, SOCK_STREAM, 0);
	if (m_sck == INVALID_SOCKET) {
		throw std::runtime_error("ソケットの作成に失敗しました");
	}
}

/*
	TCP通信用ソケットを相手に接続する関数
	引数1: const std::string& dst_ip_addr 通信相手のIPアドレス
	引数2: const uint16_t dst_port_no 通信相手のポート番号
	返り値なし
*/
void CSocket::connect_TCP_socket(const std::string& dst_ip_addr, const uint16_t dst_port_no) const
{
	sockaddr_in sa = { 0 };
	int ercd;

	sa.sin_family = AF_INET;
	sa.sin_port = htons(dst_port_no);
	ercd = inet_pton(AF_INET, dst_ip_addr.c_str(), &sa.sin_addr);
	if (ercd <= 0) {
		throw std::runtime_error("IPアドレスが不正です");
	}

	ercd = connect(m_sck, reinterpret_cast<sockaddr*>(&sa), sizeof(sa));
	if (ercd < 0) {
		throw std::runtime_error("サーバとの接続に失敗しました");
	}
}

/*
	winsockの終了処理を行う関数
	引数なし
	返り値なし
	備考: この関数は，windows環境以外の環境では何もしない
*/
void CSocket::finalize_winsock() const
{
#ifdef WIN32
	WSACleanup();
#endif // WIN32
}