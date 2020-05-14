/**
*	@file		server_socket.h
*	@brief		heis TCP/IP通信用ソケットクラス(サーバ用)
*	@author		Ryo Konno
*	@details	TCP/IPソケットの操作をより抽象化したインターフェイスを提供する(サーバ用)．
*/
#pragma once

#include <string>
#include <vector>
#include <map>

/**
*	@brief	サーバ用TCP/IPソケットクラス
*/
class CServerSocket {
	// 構造体，列挙体など
	private:
		/**
		*	@enum SocketConstVal
		*	ソケットに関する諸定数
		*/
		enum SocketConstVal {
			//! 受信バッファサイズ
			SocketConstVal_RecvBufSize = 1000,
			//! 同時接続要求の最大待ち受け数
			SocketConstVal_ConnectReqQueueLen = 5,
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

		// 送受信(送信先指定あり)
		void sck_sendto(const std::string& msg, const std::string& clt_ip_addr = "", const uint16_t clt_port_no = 0) const;
		std::string sck_recvfrom(const std::string& clt_ip_addr = "", const uint16_t clt_port_no = 0) const;

		// クライアントの情報取得
		std::vector<std::pair<std::string, uint16_t>> get_connected_client_info() const;

	private:
		// ソケット生成
		void sck_socket();

		// ソケット閉鎖
		void sck_close() const;

		// 初期化，終了処理
		void initialize_socket() const;
		void finalize_socket() const;

		// 送受信処理
		std::string sck_recv_core_win(const int sck_com) const;
		std::string sck_recv_core_linux(const int sck_com) const;

		// クライアントのIPアドレスとポート番号 -> 通信用ソケットの変換
		int client_info_to_socket(const std::string& clt_ip_addr, const uint16_t clt_port_no) const;

	// メンバ変数
	private:
		//! ソケットの実体(接続受け付け用)
		int m_sck_accept;
		//! 接続先の情報(IPアドレスとポート番号 -> 通信用ソケット)
		std::map<std::pair<std::string, uint16_t>, int> m_dest_info;
};