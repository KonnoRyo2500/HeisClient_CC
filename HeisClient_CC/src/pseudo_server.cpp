// heis ローカルモード用疑似サーバクラス
// Author: Ryo Konno

#include "pseudo_server.h"
#include "JSON_analyzer.h"

/* public関数 */

/*
	名前要求を行う「メッセージ」JSONを作成する関数
	引数なし
	返り値: std::string 名前要求を行う「メッセージ」JSON
*/
std::string CPseudoServer::send_name_req_message_json() const
{
	// TODO: 名前要求の際に入る正確なメッセージが判明したら，それを入れるようにする
	return "{\"message\": \"name_request\"}";
}

/*
	「名前確定」JSONを作成する関数
	引数なし
	返り値: std::string 「名前確定」JSON
*/
std::string CPseudoServer::send_name_decided_json() const
{
	// 具体的なチーム名はCGameLocalで定義されているので，ここではチーム名を考慮する必要はない
	return "\"your_team\": \"XXX\"";
}

/*
	「結果」JSONを作成する関数
	引数なし
	返り値: std::string 「結果」JSON
*/
std::string CPseudoServer::send_result_json() const
{
	return "\"result\": []";
}

/*
	「名前」JSONを受け取る関数
	引数1: const std::string& name_json 「名前」JSON
	返り値なし
*/
void CPseudoServer::recv_name_json(const std::string& name_json) const
{
	// 処理なし(受け取ったJSONは捨てる)
}

/*
	「行動」JSONを受け取る関数
	引数1: const std::string& name_json 「行動」JSON
	返り値なし
*/
void CPseudoServer::recv_action_json(const std::string& action_json) const
{
	// 処理なし(受け取ったJSONは捨てる)
}

/* private関数 */
