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
			//! 送信時の待ち時間(ms単位, 相手が受信中に再度送信することを防ぐための待ち時間)
			SocketConstVal_SendIntervalTimeMs = 50,
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
		void sck_send(const std::string& data) const;
		std::string sck_recv() const;

	private:
		// 初期化，終了処理
		void initialize_socket() const;
		void finalize_socket() const;

		// ソケット生成
		void sck_socket();

		// プラットフォーム別の受信処理
		std::string sck_recv_core_win() const;			// Windows用
		std::string sck_recv_core_linux() const;		// Linux用

	// メンバ変数
	private:
		//! ソケットの実体
		int m_sck;
};