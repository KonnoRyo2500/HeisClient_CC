// heis TCP/IP通信用ソケットクラス
// Author: Ryo Konno
#pragma once
class CSocket {
	// 構造体，列挙体など

	// メンバ関数
	public:
		// コンストラクタ
		explicit CSocket();
		// デストラクタ
		~CSocket();

	private:
		void initialize_winsock();
		void finalize_winsock();


	// メンバ変数
	private:
};