﻿// heis TCP/IP通信用ソケットクラス(クライアント用)
// Author: Ryo Konno
#pragma once

#include <string>

// TODO: このクラスを共通ソースに移動させる
// TODO: ソケットの基本クラスを作成し，それを継承するようにする
class CClientSocket {
	// 構造体，列挙体など
	private:
		// ソケットに関する諸定数
		enum SocketConstVal {
			SocketConstVal_RecvBufSize = 10,		// 受信バッファサイズ
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
		void sck_send(const std::string& msg) const;
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
		// ソケットの実体
		int m_sck;
};