// heis TCP/IP通信用ソケットクラス
// Author: Ryo Konno
#include "socket.h"
#include "heis_client_exception.h"
#ifdef WIN32
#include <winsock2.h>
#include <WS2tcpip.h>
#else
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <cstring>
#endif // WIN32
#include <stdexcept>

/* public関数 */

/*
	コンストラクタ
	引数なし
*/
CSocket::CSocket()
	: m_sck(0)
{
	// Windows環境で動作させる場合，ソケット通信にwinsockを使うので，その初期化を行う(windows環境以外ならば何もしない)
	initialize_TCP_socket();
	make_TCP_socket();
}

/*
	デストラクタ
	引数なし
*/
CSocket::~CSocket()
{
	// Windows環境で動作させる場合，ソケット通信にwinsockを使うので，その終了処理を行う
	finalize_TCP_socket();
}

/*
	TCP通信用ソケットを相手に接続する関数
	引数1: const std::string& dst_ip_addr 通信相手のIPアドレス
	引数2: const uint16_t dst_port_no 通信相手のポート番号
	返り値なし
*/
void CSocket::sck_connect(const std::string& dst_ip_addr, const uint16_t dst_port_no) const
{
	sockaddr_in sa = { 0 };
	int ercd;

	sa.sin_family = AF_INET;
	sa.sin_port = htons(dst_port_no);
	ercd = inet_pton(AF_INET, dst_ip_addr.c_str(), &sa.sin_addr);
	if (ercd <= 0) {
		throw CHeisClientException("IPアドレス\"%s\"は不正です", dst_ip_addr.c_str());
	}

	ercd = connect(m_sck, reinterpret_cast<sockaddr*>(&sa), sizeof(sa));
	if (ercd < 0) {
		throw CHeisClientException("サーバとの接続に失敗しました(エラーコード: %d)", errno);
	}
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
			throw CHeisClientException("送信でエラーが発生しました(エラーコード: %d)", errno);
		}
		fprintf(stderr, "警告: 不完全なメッセージが送信されました(%d文字中%d文字が送信されました)\n", msg.size(), send_size);
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
#ifdef WIN32
	return recv_from_core_win();
#else
	return recv_from_core_linux();
#endif
}

/* private関数 */

/*
	ソケットを初期化する関数
	引数なし
	返り値なし
	例外: ソケットの初期化に失敗したとき
*/
void CSocket::initialize_TCP_socket() const
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
	TCP通信用ソケットを作成する関数
	引数なし
	返り値なし
	例外: ソケットの作成に失敗したとき
*/
void CSocket::make_TCP_socket()
{
	m_sck = socket(AF_INET, SOCK_STREAM, 0);
	if (m_sck < 0) {
		throw CHeisClientException("ソケットの作成に失敗しました(エラーコード: %d)", errno);
	}
}

/*
	winsockの終了処理を行う関数
	引数なし
	返り値なし
	備考: この関数は，windows環境以外の環境では何もしない
*/
void CSocket::finalize_TCP_socket() const
{
#ifdef WIN32
	WSACleanup();
	closesocket(m_sck);
#else
	close(m_sck);
#endif // WIN32
}

// 受信処理に関しては、#ifdefが関数中に入り乱れるのを防ぐため、プラットフォーム別に関数を分ける
// REQUEST: プラットフォームに依らない受信処理の実装
/*
	受信処理(Windows向け)
	引数なし
	返り値: std::string 受信したメッセージ
	例外: 受信エラーが発生したとき
*/
std::string CSocket::recv_from_core_win() const
{
#ifdef WIN32
	// メッセージを確実にNULL終端させるため，バッファは1バイト余分に取る
	char buf[SocketConstVal_RecvBufSize + 1] = { 0 };
	int recv_size;
	std::string recv_message;

	// データの到着前に抜けてしまうのを防ぐため，最初の受信はブロッキングにする
	recv_size = recv(m_sck, buf, sizeof(buf) - 1, 0);
	if (recv_size < 0) {
		throw CHeisClientException("受信でエラーが発生しました(エラーコード: %d)", errno);
	}
	recv_message += std::string(buf);

	// 入力キューにデータが残っていれば，それらもすべて受信する
	{
		// 受信データがないときに無限待ちにならないよう，一旦ソケットをノンブロッキングにする
		unsigned long nonblocking_enable = 1;
		ioctlsocket(m_sck, FIONBIO, &nonblocking_enable);
	}
	do {
		memset(buf, 0, sizeof(buf));
		recv_size = recv(m_sck, buf, sizeof(buf) - 1, 0);
		if (recv_size < 0) {
			if (WSAGetLastError() == WSAEWOULDBLOCK) {
				// すべて受信できたので，受信終了
				break;
			}
			else {
				throw CHeisClientException("受信でエラーが発生しました(エラーコード: %d)", errno);
			}
		}
		recv_message += std::string(buf);
	} while (recv_size > 0);
	{
		// 次の呼び出しでの最初の受信をブロッキングにするため，ソケットをブロッキングに戻す
		unsigned long nonblocking_disable = 0;
		ioctlsocket(m_sck, FIONBIO, &nonblocking_disable);
	}

	return recv_message;
#else
	return "";
#endif // WIN32
}

/*
	受信処理(Linux向け)
	引数なし
	返り値: std::string 受信したメッセージ
	例外: 受信エラーが発生したとき
*/
std::string CSocket::recv_from_core_linux() const
{
#ifndef WIN32
	// メッセージを確実にNULL終端させるため，バッファは1バイト余分に取る
	char buf[SocketConstVal_RecvBufSize + 1] = { 0 };
	int recv_size;
	std::string recv_message;

	// データの到着前に抜けてしまうのを防ぐため，最初の受信はブロッキングにする
	recv_size = recv(m_sck, buf, sizeof(buf) - 1, 0);
	if (recv_size < 0) {
		throw CHeisClientException("受信でエラーが発生しました(エラーコード: %d)", errno);
	}
	recv_message += std::string(buf);

	// 入力キューにデータが残っていれば，それらもすべて受信する
	do {
		memset(buf, 0, sizeof(buf));
		// 受信データがないときに無限待ちにならないよう，ノンブロッキングで受信する
		recv_size = recv(m_sck, buf, sizeof(buf) - 1, MSG_DONTWAIT);
		if (recv_size < 0) {
			if (errno == EAGAIN) {
				// すべて受信できたので，受信終了
				break;
			}
			else {
				throw CHeisClientException("受信でエラーが発生しました(エラーコード: %d)", errno);
			}
		}
		recv_message += std::string(buf);
	} while (recv_size > 0);

	return recv_message;
#else
	return "";
#endif // WIN32
}
