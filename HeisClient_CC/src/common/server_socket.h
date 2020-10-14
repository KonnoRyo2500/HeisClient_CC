/**
*	@file		server_socket.h
*	@brief		heis TCP/IP通信用ソケットクラス(サーバ用)
*	@author		Ryo Konno
*	@details	TCP/IPソケットの操作をより抽象化したインターフェイスを提供する(サーバ用)．
*/
#pragma once

#include <string>
#include <vector>

/**
*	@brief	サーバ用TCP/IPソケットクラス
*	@details 本クラスのインスタンス1つにつき、1つのクライアントと通信する。
*	@remark heisでは、JSON(文字列データ)のみ取り扱うため、データはstd::string型で持つものとする。
*/
class CServerSocket {
	// 構造体，列挙体など
	private:
		/**
		*	@enum SocketConstVal
		*	ソケットに関する諸定数
		*/
		enum SocketConstVal {
			//! 同時接続要求の最大待ち受け数
			SocketConstVal_ConnectReqQueueLen = 5,
			//! 受信バッファサイズ
			SocketConstVal_RecvBufSize = 1000,
			//! 送信時の待ち時間(ms単位, 相手が受信中に再度送信することを防ぐための待ち時間)
			SocketConstVal_SendIntervalTimeMs = 50,
		};

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

		// 送受信
		void sck_send(const std::string& data, const char etx = '\n') const;
		std::string sck_recv(const char etx = '\n');

	private:
		// ソケット生成
		void sck_socket();

		// ソケット閉鎖
		void sck_close() const;

		// 初期化，終了処理
		void initialize_socket() const;
		void finalize_socket() const;

	// メンバ変数
	private:
		//! ソケットの実体
		int m_sck;

		//! 前回の受信時に余分に受信したデータ
		std::string m_prev_recv_remaind_data;
};