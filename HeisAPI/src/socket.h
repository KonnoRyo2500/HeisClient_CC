// heis TCP/IP通信用ソケットクラス
// Author: Ryo Konno
#pragma once

#include <string>

class CSocket {
	// 構造体，列挙体など
	private:
		// ソケットに関する諸定数
		enum SocketConstVal {
			SocketConstVal_RecvBufSize = 10,		// 受信バッファサイズ
		};

	// メンバ関数
	public:
		// コンストラクタ
		explicit CSocket(const std::string& dst_ip_addr, const uint16_t dst_port_no);

		// デストラクタ
		~CSocket();

		// 送受信
		void send_to(const std::string& msg) const;
		std::string recv_from() const;

	private:
		// 初期化，終了処理
		void initialize_TCP_socket() const;
		void make_TCP_socket();
		void connect_TCP_socket(const std::string& dst_ip_addr, const uint16_t dst_port_no) const;
		void finalize_TCP_socket() const;

		// プラットフォーム別の受信処理
		std::string recv_from_core_win() const;			// Windows用
		std::string recv_from_core_linux() const;		// Linux用

	// メンバ変数
	private:
		int m_sck;
};