// heisクライアント用JSON送受信治具 シナリオ読み込みクラス
// Author: Ryo Konno

#include "scenario_reader.h"
#include "heis_client_exception.h"

#define SPACE_AND_TAB " \t"

/* public関数 */

/*
	コンストラクタ
	引数1: const std::string& scenario_file_name シナリオファイル名
	例外: シナリオファイルのオープンに失敗したとき
*/
CScenarioReader::CScenarioReader(const std::string& scenario_file_name)
	: m_next_action_type(ActionType_AllActionDone)
	, m_scenario_file(scenario_file_name)
{
	if(m_scenario_file.fail()){
		throw CHeisClientException("シナリオファイルのオープンに失敗しました(ファイル名: %s)", scenario_file_name.c_str());
	}
}

/*
	デストラクタ
	引数なし
*/
CScenarioReader::~CScenarioReader()
{
	// 処理なし
}

/*
	次のアクションの種類を取得する関数
	引数なし
	返り値: CScenarioReader::ActionType 次のアクションの種類
	備考: この関数を連続で呼び出すことで，シナリオファイルに記述されたアクションを順に取得できる
*/
CScenarioReader::ActionType CScenarioReader::get_next_aciton_type()
{
	std::string action_msg;
	while(getline(m_scenario_file, action_msg)){
		std::vector<std::string> action_tokens = split_action_message(action_msg);
		printf("action: %s\n", action_tokens[0].c_str());
	}
}

/* private関数 */

/*
	アクションを記述した文字列を空白で区切り，トークンに分解する関数
	引数1: const std::string& action_msg アクションを記述した文字列
	返り値: std::vector<std::string> 分割の結果得られたトークン
*/
std::vector<std::string> CScenarioReader::split_action_message(const std::string& action_msg)
{
	std::string action_msg_work(action_msg);
	std::vector<std::string> tokens;
	
	while(action_msg_work.size() > 0){
		std::string token;
		delete_front_space(action_msg_work);
		token = cut_front_token(action_msg_work);
		if(token.size() > 0){
			tokens.push_back(token);
		}
	}
	return tokens;
}

/*
	与えられた文字列の先頭にある空白列を削除する関数
	参照1: std::string& src_str 文字列
	返り値なし
	備考: 文字列がすべて空白で構成されていた場合，文字列を空文字列にする
*/
void CScenarioReader::delete_front_space(std::string& src_str)
{
	// 次に出現するトークンの先頭位置
	size_t next_token_pos = src_str.find_first_not_of(SPACE_AND_TAB);

	if(next_token_pos == std::string::npos){
		src_str = "";
		return;
	}
	src_str = src_str.substr(next_token_pos);
}

/*
	与えられた文字列の先頭にある文字列を空白が出現するまで切り取る関数
	参照1: std::string& src_str 文字列
	返り値: std::string 切り取った文字列
	備考: 与えられた文字列に空白がない場合，元の文字列をそのまま返し，文字列を空文字列にする
*/
std::string CScenarioReader::cut_front_token(std::string& src_str)
{
	// 次に出現する空白の先頭位置
	size_t next_spaces_pos = src_str.find_first_of(SPACE_AND_TAB);
	
	if(next_spaces_pos == std::string::npos){
		next_spaces_pos = src_str.size();
	}
	std::string token = src_str.substr(0, next_spaces_pos);
	src_str = src_str.substr(next_spaces_pos);
	return token;
}
