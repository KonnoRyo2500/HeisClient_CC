// heis TCP/IP通信用ソケットクラス(サーバ用)
// Author: Ryo Konno
#pragma once

#include <string>
#include <vector>

// TODO: このクラスを共通ソースに移動させる
class CServerSocket {
	// 構造体，列挙体など
	private:
		// ソケットに関する諸定数
		enum SocketConstVal {
			SocketConstVal_RecvBufSize = 10,		// 受信バッファサイズ
		};

	// メンバ関数
	public:
		// コンストラクタ
		explicit CServerSocket();

		// デストラクタ
		~CServerSocket();

		// クライアントからの接続待ち
		void sck_accept();
		void sck_listen();

		// 送受信
		void sck_send(const std::string& msg) const;
		std::string sck_recv() const;

	private:
		// 初期化，終了処理
		void initialize_socket() const;
		void sck_socket();
		void finalize_socket() const;

	// メンバ変数
	private:
		// ソケットの実体(接続受け付け用)
		int m_sck_accept;
		// ソケットの実体(クライアントとの通信用)
		std::vector<int> m_sck_com;
};