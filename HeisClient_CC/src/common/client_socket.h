/**
*	@file		client_socket.h
*	@brief		TCP/IP通信用ソケットクラス(クライアント用)
*	@author		Ryo Konno
*	@details	TCP/IPソケットの操作をより抽象化したインターフェイスを提供する(クライアント用)．
*/
#pragma once

#include "socket_base.h"

#include <string>

/**
*	@brief	クライアント用TCP/IPソケットクラス
*	@remark heisでは、JSON(文字列データ)のみ取り扱うため、データはstd::string型で持つものとする。
*/
class CClientSocket : public CSocketBase {
	// メンバ関数
	public:
		// コンストラクタ
		explicit CClientSocket();
		// デストラクタ
		~CClientSocket();

		// サーバと接続
		void sck_connect(const std::string& dst_ip_addr, const uint16_t dst_port_no) const;

	private:

	// メンバ変数
	private:
		// なし
};