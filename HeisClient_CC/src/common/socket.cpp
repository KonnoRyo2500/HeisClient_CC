/**
*	@file		socket.cpp
*	@brief		TCP/IPソケットクラス
*	@author		Ryo Konno
*	@details	プラットフォームに依らないTCP/IPソケット操作を提供する。
*/

#include "socket.h"
#include "windows_socket.h"
#include "linux_socket.h"

#include <stdexcept>
#include <assert.h>

/**
*	@brief コンストラクタ
*	@details socket処理が行われる
*	@throws runtime_error OSがサポートされていない場合
*/
CSocket::CSocket()
{
	// プラットフォームに応じて、ソケットクラスのインスタンスを作成する
#if defined(WIN32)
	m_socket = new CWindowsSocket();
#elif defined(__linux)
	m_socket = new CLinuxSocket();
#else
	m_socket = NULL;
#endif

	if (m_socket == NULL) {
		throw std::runtime_error("ソケットクラスはお使いのOSをサポートしていません");
	}
}

/**
*	@brief ソケットに名前を付ける
*	@param[in] dst_port 接続先ポート番号
*	@param[in] src_addr 接続元IPアドレス
*/
void CSocket::bind(const uint16_t dst_port, const std::string& src_addr)
{
	assert(m_socket != NULL);
	m_socket->wrap_bind(dst_port, src_addr);
}

/**
*	@brief 相手からの接続を受け付ける
*/
void CSocket::listen()
{
	assert(m_socket != NULL);
	m_socket->wrap_listen();
}

/**
*	@brief 相手からの接続を待つ
*/
void CSocket::accept()
{
	assert(m_socket != NULL);
	m_socket->wrap_accept();
}

/**
*	@brief ソケットを接続する
*	@param[in] addr 接続先IPアドレス
*	@param[in] port 接続先ポート番号
*/
void CSocket::connect(const std::string& addr, const uint16_t port) const
{
	assert(m_socket != NULL);
	m_socket->wrap_connect(addr, port);
}

/**
*	@brief データを送信する
*	@param[in] data 送信するデータ
*	@param[in] terminal 終端文字(省略時は付与しない)
*/
void CSocket::send(const std::string& data, const char terminal)
{
	assert(m_socket != NULL);
	m_socket->wrap_send(data, terminal);
}

/**
*	@brief データを受信する
*	@param[in] terminal 終端文字(省略時は付与しない)
*/
std::string CSocket::recv(const char terminal)
{
	assert(m_socket != NULL);
	return m_socket->wrap_recv(terminal);
}

/**
*	@brief ソケットを閉じる
*/
void CSocket::close()
{
	assert(m_socket != NULL);
	m_socket->wrap_close();

	// ソケットの終了処理や破棄もここで行う
	// ソケットの終了処理
	bool finalize_result = m_socket->finalize();
	if (!finalize_result) {
		throw std::runtime_error("ソケットの終了処理に失敗しました");
	}

	// ソケットの破棄
	delete m_socket;
	m_socket = NULL;
}