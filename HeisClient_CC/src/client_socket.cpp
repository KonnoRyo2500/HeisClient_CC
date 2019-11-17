// heis TCP/IP通信用ソケットクラス(クライアント用)
// Author: Ryo Konno
#include "client_socket.h"
#include "heis_client_exception.h"

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

/*
	コンストラクタ
	引数なし
*/
CClientSocket::CClientSocket()
	: m_sck(0)
{
	initialize_socket();
	sck_socket();
}

/*
	デストラクタ
	引数なし
*/
CClientSocket::~CClientSocket()
{
	finalize_socket();
}

/*
	TCP通信用ソケットを相手に接続する関数
	引数1: const std::string& dst_ip_addr 通信相手のIPアドレス
	引数2: const uint16_t dst_port_no 通信相手のポート番号
	返り値なし
*/
void CClientSocket::sck_connect(const std::string& dst_ip_addr, const uint16_t dst_port_no) const
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
void CClientSocket::sck_send(const std::string& msg) const
{
	// サーバが前回送信されたメッセージを受信中に再度送信することを防ぐため，少しだけ待ちを入れる
#ifdef WIN32
	Sleep(SocketConstVal_SendIntervalTimeMs);
#else
	usleep(SocketConstVal_SendIntervalTimeMs * 1000);
#endif

	// メッセージを確実にNULL終端させるため，size + 1文字送信する
	size_t send_size = send(m_sck, msg.c_str(), msg.size() + 1, 0);

	if (send_size < msg.size()) {
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
std::string CClientSocket::sck_recv() const
{
#ifdef WIN32
	return sck_recv_core_win();
#else
	return sck_recv_core_linux();
#endif
}

/* private関数 */

/*
	ソケットを初期化する関数
	引数なし
	返り値なし
	例外: ソケットの初期化に失敗したとき
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
		throw CHeisClientException("winsockの初期化に失敗しました(エラーコード: %d)", init_ercd);
	}
#endif // WIN32
}

/*
	winsockの終了処理を行う関数
	引数なし
	返り値なし
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

/*
	TCP通信用ソケットを作成する関数
	引数なし
	返り値なし
	例外: ソケットの作成に失敗したとき
*/
void CClientSocket::sck_socket()
{
	m_sck = socket(AF_INET, SOCK_STREAM, 0);
	if (m_sck < 0) {
		throw CHeisClientException("ソケットの作成に失敗しました(エラーコード: %d)", errno);
	}
}

// 受信処理に関しては、#ifdefが関数中に入り乱れるのを防ぐため、プラットフォーム別に関数を分ける
// TODO: プラットフォームに依らない受信処理の実装
// TODO: 受信中に相手が新たに送信してきても，新たに送信されたメッセージをまとめて受信しないようにする
/*
	受信処理(Windows向け)
	引数なし
	返り値: std::string 受信したメッセージ
	例外: 受信エラーが発生したとき
*/
std::string CClientSocket::sck_recv_core_win() const
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
	while (recv_size == sizeof(buf) - 1) {
		memset(buf, 0, sizeof(buf));
		recv_size = recv(m_sck, buf, sizeof(buf) - 1, 0);
		recv_message += std::string(buf);
	}
	// 次の呼び出しでの最初の受信をブロッキングにするため，ソケットをブロッキングに戻す
	{
		unsigned long nonblocking_disable = 0;
		ioctlsocket(m_sck, FIONBIO, &nonblocking_disable);
	}

	if (recv_size < 0) {
		if (WSAGetLastError() != WSAEWOULDBLOCK) {
			throw CHeisClientException("受信でエラーが発生しました(エラーコード: %d)", errno);
		}
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
std::string CClientSocket::sck_recv_core_linux() const
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
	while (recv_size == sizeof(buf) - 1) {
		memset(buf, 0, sizeof(buf));
		// 受信データがないときに無限待ちにならないよう，ノンブロッキングで受信する
		recv_size = recv(m_sck, buf, sizeof(buf) - 1, MSG_DONTWAIT);
		recv_message += std::string(buf);
	};

	if (recv_size < 0) {
		if (errno != EAGAIN) {
			throw CHeisClientException("受信でエラーが発生しました(エラーコード: %d)", errno);
		}
	}

	return recv_message;
#else
	return "";
#endif // WIN32
}
