// heisクライアント用JSON送受信治具 シナリオ読み込みクラス
// Author: Ryo Konno

#include <map>

#include "scenario_reader.h"
#include "heis_client_exception.h"

#define SPACE_AND_TAB " 　\t"

/* public関数 */

/*
	コンストラクタ
	引数1: const std::string& scenario_file_name シナリオファイル名
	例外: シナリオファイルのオープンに失敗したとき
*/
CScenarioReader::CScenarioReader(const std::string& scenario_file_name)
	: m_scenario_file(scenario_file_name)
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
	std::string action_str;
	// コマンドとアクションの種類の対応表
	const std::map<token_array_t, ActionType> command_actiontype_map = {
		{{"recv", "print"},		ActionType_PrintRecvMessage	},
		{{"recv", "write"},		ActionType_WriteRecvMessage	},
		{{"send", "msg"},		ActionType_SendMessage		},
		{{"send", "file"},		ActionType_SendFileContents	},
	};

	// ファイルをすべて読みきっていれば，それ以上は何もしない
	if(m_scenario_file.eof()){
		return ActionType_AllActionDone;
	}
	
	// シナリオファイルから読み出し，トークンに分割
	getline(m_scenario_file, action_str);
	CTokenManager tm;
	token_array_t action = tm.split_string(action_str, SPACE_AND_TAB);
	m_latest_action = action;

	// アクションの種類を判定
	if(is_empty_line(action_str)){
		return ActionType_None;
	}
	for(auto& cmd_actiontype : command_actiontype_map){
		if(is_match_command_part(action, cmd_actiontype.first)){
			return cmd_actiontype.second;
		}
	}
	return ActionType_Invalid;
}

/*
	"send msg"アクションで送信するメッセージを取得する関数
	引数なし
	返り値: std::string 送信するメッセージ
	例外: メッセージを取得できないとき
*/
std::string CScenarioReader::get_message_to_send() const
{
	// TODO: 空白を含むメッセージにも対応できるようにする
	if(m_latest_action.size() >= 3){
		CTokenManager tm;
		return tm.get_single_token(m_latest_action, 2);
	}
	throw CHeisClientException("メッセージが指定されていない，もしくはアクションが異なります");
}

/*
	"send file"アクションで送信するメッセージが記載されているファイル名を取得する関数
	引数なし
	返り値: std::string 送信するメッセージが記載されているファイル名
	例外: ファイル名を取得できないとき
*/
std::string CScenarioReader::get_filename_to_send() const
{
	if(m_latest_action.size() >= 3){
		CTokenManager tm;
		return tm.get_single_token(m_latest_action, 2);
	}
	throw CHeisClientException("ファイル名が指定されていない，もしくはアクションが異なります");
}

/*
	"recv write"アクションで受信したメッセージを書き出すファイル名を取得する関数
	引数なし
	返り値: std::string 受信したメッセージを書き出すファイル名
	例外: ファイル名を取得できないとき
*/
std::string CScenarioReader::get_filename_to_write_recv_msg() const
{
	if(m_latest_action.size() >= 3){
		CTokenManager tm;
		return tm.get_single_token(m_latest_action, 2);
	}
	throw CHeisClientException("ファイル名が指定されていない，もしくはアクションが異なります");
}

/* private関数 */
/*
	与えられたアクションのコマンド部分が，与えられたコマンドに一致しているかを判定する関数
	引数1: const token_array_t action アクション
	引数2: const token_array_t command コマンド
	返り値: bool actionのコマンド部分が，commandと一致しているか(一致: true, 不一致: false)
*/
bool CScenarioReader::is_match_command_part(const token_array_t action, const token_array_t command) const
{
	if(action.size() < command.size()){
		return false;
	}
	int i = 0;
	for(auto& token : command){
		if(token != action[i]){
			return false;
		}
		i++;
	}
	return true;
}

/*
	与えられた行(アクション)が空行かどうかを判定する関数
	引数1: const std::string& action_str アクション
	返り値: bool action_strが空行か(true: 空行, false: 空行ではない)
	備考: 「空行」とは空白類(スペースもしくはタブ)で構成された行のことを指す
*/
bool CScenarioReader::is_empty_line(const std::string& action_str) const
{
	return action_str.find_first_not_of(SPACE_AND_TAB) == std::string::npos;
}
