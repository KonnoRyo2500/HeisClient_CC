/**
*	@file		socket.h
*	@brief		TCP/IPソケットクラス
*	@author		Ryo Konno
*	@details	プラットフォームに依らないTCP/IPソケット操作を提供する。
*/

#pragma once

#include "socket_base.h"

#include <string>

/**
*	@brief	TCP/IPソケットクラス
*/
class CSocket
{
	// メンバ変数
public:
	// コンストラクタ
	explicit CSocket();

	// ソケットに名前を付ける
	void bind(const uint16_t dst_port, const std::string& src_addr);
	// 相手からの接続を受け付ける
	void listen();
	// 相手からの接続を待つ
	void accept();
	// ソケットを接続する
	void connect(const std::string& addr, const uint16_t port) const;
	// データを送信する
	void send(const std::string& data, const char terminal = '\0');
	// データを受信する
	std::string recv(const char terminal = '\0');
	// ソケットを閉じる
	void close();

	// socketはCSocketBaseクラスのコンストラクタで呼ばれる
	// 定義も、CSocketBaseクラスを継承したクラスでされている

	// メンバ変数
private:
	//! ソケットの実体
	CSocketBase* m_socket;
};