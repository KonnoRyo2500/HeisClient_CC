// heis ローカルモード用疑似サーバクラス
// Author: Ryo Konno
#pragma once

#include <string>
#include <vector>

// クラス名に"Server"とついてはいるが，通信は行わない
// ただし，実際のサーバと同様に，情報のやり取りはJSONで行う
class CPseudoServer
{
	// 構造体，列挙体など

	// メンバ関数
	public:
		// JSON送信(作成)
		std::string send_name_req_message_json() const;
		std::string send_name_decided_json() const;
		std::string send_initial_field_json() const;
		std::string send_field_json() const;
		std::string send_result_json() const;

		// JSON受信(受け取り)
		void recv_name_json(const std::string& name_json) const;
		void recv_action_json(const std::string& action_json) const;

	private:

	// メンバ変数
	private:
};