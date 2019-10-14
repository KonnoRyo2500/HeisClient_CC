// heisクライアント用JSON送受信治具 シナリオ読み込みクラス
// Author: Ryo Konno

#include "scenario_reader.h"
#include "heis_client_exception.h"
#include <map>

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
	const std::map<std::vector<std::string>, ActionType> command_actiontype_map = {
		{{"recv"},				ActionType_Receive			},
		{{"send", "msg"},		ActionType_SendMessage		},
		{{"send", "file"},		ActionType_SendFileContents	},
	};

	if(m_scenario_file.eof()){
		return ActionType_AllActionDone;
	}
	
	// シナリオファイルから読み出し，トークンに分割
	getline(m_scenario_file, action_str);
	action_str = build_no_control_code_string(action_str);
	std::vector<std::string> action = split_action_string(action_str);
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
		return m_latest_action[2];
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
		return m_latest_action[2];
	}
	throw CHeisClientException("ファイル名が指定されていない，もしくはアクションが異なります");
}

/* private関数 */

/*
	アクションを記述した文字列を空白で区切り，トークンに分解する関数
	引数1: const std::string& action_str アクションを記述した文字列
	返り値: std::vector<std::string> 分割の結果得られたトークン
*/
std::vector<std::string> CScenarioReader::split_action_string(const std::string& action_str) const
{
	std::string action_str_work(action_str);
	std::vector<std::string> tokens;
	
	while(action_str_work.size() > 0){
		std::string token;
		erase_front_space(action_str_work);
		token = cut_front_token(action_str_work);
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
void CScenarioReader::erase_front_space(std::string& src_str) const
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
std::string CScenarioReader::cut_front_token(std::string& src_str) const
{
	// 次に出現する空白の先頭位置
	size_t next_spaces_pos = src_str.find_first_of(SPACE_AND_TAB);
	
	if(next_spaces_pos == std::string::npos){
		next_spaces_pos = src_str.size();
	}
	//std::string token = erase_BOM_and_CR(src_str.substr(0, next_spaces_pos));
	std::string token = src_str.substr(0, next_spaces_pos);
	src_str = src_str.substr(next_spaces_pos);
	return token;
}

/*
	与えられた文字列から不要な制御文字を取り除いた文字列を作成する関数
	引数1: std::string& src_str 文字列
	返り値: std::string src_strから制御文字を取り除いた文字列
	備考: 本関数は，シナリオファイルから取り出したアクションに余計な制御文字が含まれてしまうことを防ぐための関数である
*/
std::string CScenarioReader::build_no_control_code_string(const std::string& src_str) const
{
	// 削除する制御文字一覧
	const std::vector<std::string> ctl_codes = {
		// BOM(UTF-8)
		/*
		  この処理により，SJISで「*ｻｿ」(*は2バイト目が0xEFの文字)を含む文字列は不正な文字列になるが，
		  そのような文字列を含むアクションはユーザが運用によりシナリオファイルに記述しないものとする．
		*/
		"\xEF\xBB\xBF",
		// CR
		"\r",
		// LF
		"\n",
	};
	std::string no_control_code_str = src_str;

	for(auto ctl_code : ctl_codes){
		erase_substring(no_control_code_str, ctl_code);
	}
	return no_control_code_str;
}

/*
	指定された文字列を，文字列中から削除する関数
	参照1: std::string& dst_str 加工対象の文字列
	引数1: const std::string& erase_str 削除する文字列
	返り値なし
*/
void CScenarioReader::erase_substring(std::string& dst_str, const std::string& erase_str) const
{
	size_t substr_pos = dst_str.find(erase_str);
	while(substr_pos != std::string::npos){
		dst_str.erase(substr_pos, erase_str.size());
		substr_pos = dst_str.find(erase_str);
	}
}

/*
	与えられたアクションのコマンド部分が，与えられたコマンドに一致しているかを判定する関数
	引数1: const std::vector<std::string> action アクション
	引数2: const std::vector<std::string> command コマンド
	返り値: bool actionのコマンド部分が，commandと一致しているか(一致: true, 不一致: false)
*/
bool CScenarioReader::is_match_command_part(const std::vector<std::string> action, const std::vector<std::string> command) const
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
