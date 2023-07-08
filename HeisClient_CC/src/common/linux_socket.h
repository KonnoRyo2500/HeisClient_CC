/**
*	@file		linux_socket.h
*	@brief		Linux用TCP/IPソケットクラス
*	@author		Ryo Konno
*	@details	Linux環境におけるソケットの各種操作を提供する。
*/

#pragma once

#ifdef __linux

#include "socket_base.h"

/**
*	@brief Linux用TCP/IPソケットクラス
*/
class CLinuxSocket : public CSocketBase
{
	// メンバ関数
public:
	// コンストラクタ
	CLinuxSocket();

	// ソケットに名前を付ける
	void wrap_bind(const uint16_t dst_port, const std::string& src_addr);
	// 相手からの接続を受け付ける
	void wrap_listen();
	// 相手からの接続を待つ
	void wrap_accept();
	// ソケットを接続する
	void wrap_connect(const std::string& addr, const uint16_t port) const;
	// データを送信する
	void wrap_send(const std::string& data, const char terminal);
	// データを受信する
	std::string wrap_recv(const char terminal);
	// ソケットを閉じる
	void wrap_close();
	// ソケットを作成する
	int wrap_socket();

	// 初期化処理を行う
	bool initialize();
	// 終了処理を行う
	bool finalize();
};

#endif // __linux