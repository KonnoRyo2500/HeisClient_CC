// heisクライアント用JSON送受信治具 送信クラス
// Author: Ryo Konno

#include <string>

#include "server_socket.h"

extern CServerSocket g_sck;

class CJsonSender {
	// メンバ関数
	public:
		void send_JSON(const std::string& JSON) const;
		void send_JSON_from_file(const std::string& JSON_file_name) const;

	private:

	// メンバ変数
	private:
		// なし
};
