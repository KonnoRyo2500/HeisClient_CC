/**
*	@file		client_socket.h
*	@brief		TCP/IP通信用ソケットクラス(クライアント用)
*	@author		Ryo Konno
*	@details	TCP/IPソケットの操作をより抽象化したインターフェイスを提供する(クライアント用)．
*/
#pragma once

#include <string>

/**
*	@brief	クライアント用TCP/IPソケットクラス
*	@remark heisでは、JSON(文字列データ)のみ取り扱うため、データはstd::string型で持つものとする。
*/
class CClientSocket {
	// 構造体，列挙体など
	private:
		/**
		*	@enum SocketConstVal
		*	ソケットに関する諸定数
		*/
		enum SocketConstVal {
			//! 受信バッファサイズ
			SocketConstVal_RecvBufSize = 1000,
		};

	// メンバ関数
	public:
		// コンストラクタ
		explicit CClientSocket();

		// デストラクタ
		~CClientSocket();

		// サーバと接続
		void sck_connect(const std::string& dst_ip_addr, const uint16_t dst_port_no) const;

		// 送受信
		void sck_send(const std::string& data, const char etx = '\n') const;
		std::string sck_recv(const char etx = '\n');

	private:
		// 初期化，終了処理
		void initialize_socket() const;
		void finalize_socket() const;

		// ソケット生成
		void sck_socket();

	// メンバ変数
	private:
		//! ソケットの実体
		int m_sck;

		//! 前回の受信時に余分に受信したデータ
		std::string m_prev_recv_remaind_data;
};