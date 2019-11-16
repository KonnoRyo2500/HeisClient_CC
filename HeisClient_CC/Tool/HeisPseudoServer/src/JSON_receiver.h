// heisクライアント用JSON送受信治具 受信クラス
// Author: Ryo Konno

#include "server_socket.h"

extern CServerSocket g_sck;

class CJsonReceiver {
	// メンバ関数
	public:
		void recv_JSON_and_print() const;
		void recv_JSON_and_write_file(const std::string& JSON_file_name) const;

	private:
		

	// メンバ変数
	private:
		// なし
};
