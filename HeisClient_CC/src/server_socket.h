// heis TCP/IP通信用ソケットクラス(サーバ用)
// Author: Ryo Konno
#pragma once

#include <string>
#include <vector>

// TODO: このクラスを共通ソースに移動させる
// TODO: ソケットの基本クラスを作成し，それを継承するようにする
class CServerSocket {
	// 構造体，列挙体など
	private:
		// ソケットに関する諸定数
		enum SocketConstVal {
			SocketConstVal_RecvBufSize = 10,		// 受信バッファサイズ
			SocketConstVal_ConnectReqQueueLen = 5,	// 同時接続要求の最大待ち受け数
		};

	// メンバ関数
	public:
		// コンストラクタ
		explicit CServerSocket();

		// デストラクタ
		~CServerSocket();

		// 自身のアドレスとポート番号をソケットに紐づけ
		void sck_bind(const uint16_t svr_port_no, const std::string clt_ip_addr = "0.0.0.0") const;

		// クライアントからの接続待ち
		void sck_listen() const;
		void sck_accept();

		// 送受信
		void sck_send(const std::string& msg) const;
		std::string sck_recv() const;

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
		// ソケットの実体(接続受け付け用)
		int m_sck_accept;
		// ソケットの実体(クライアントとの通信用)
		std::vector<int> m_sck_com;
		// 接続しているクライアントの情報(IPアドレスとポート番号)
		std::vector<std::pair<std::string, uint16_t>> m_client_info;
};