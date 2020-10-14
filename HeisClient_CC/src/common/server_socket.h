/**
*	@file		server_socket.h
*	@brief		heis TCP/IP通信用ソケットクラス(サーバ用)
*	@author		Ryo Konno
*	@details	TCP/IPソケットの操作をより抽象化したインターフェイスを提供する(サーバ用)．
*/
#pragma once

#include "socket_base.h"

#include <string>
#include <vector>

/**
*	@brief	サーバ用TCP/IPソケットクラス
*	@details 本クラスのインスタンス1つにつき、1つのクライアントと通信する。
*	@remark heisでは、JSON(文字列データ)のみ取り扱うため、データはstd::string型で持つものとする。
*/
class CServerSocket : public CSocketBase{
	// メンバ関数
	public:
		// コンストラクタ
		explicit CServerSocket();

		// デストラクタ
		~CServerSocket();

		// 自身のアドレスとポート番号をソケットに紐づけ
		void sck_bind(const uint16_t svr_port_no, const std::string& clt_ip_addr = "0.0.0.0") const;

		// クライアントからの接続待ち
		void sck_listen() const;
		void sck_accept();

	private:

	// メンバ変数
	private:
		// なし
};