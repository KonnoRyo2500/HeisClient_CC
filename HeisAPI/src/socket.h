// heis TCP/IP通信用ソケットクラス
// Author: Ryo Konno
#pragma once

#include <string>

class CSocket {
	// 構造体，列挙体など
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
		void initialize_winsock() const;
		void make_TCP_socket();
		void connect_TCP_socket(const std::string& dst_ip_addr, const uint16_t dst_port_no) const;
		void finalize_winsock() const;


	// メンバ変数
	private:
		int m_sck;
};