// heisクライアント用JSON送受信治具 シナリオ読み込みクラス
// Author: Ryo Konno

#include "scenario_reader.h"
#include "heis_client_exception.h"

/* public関数 */

/*
	コンストラクタ
	引数1: const std::string& scenario_file_name シナリオファイル名
	例外: シナリオファイルのオープンに失敗したとき
*/
CScenarioReader::CScenarioReader(const std::string& scenario_file_name)
	: m_next_action_type(ActionType_AllActionDone)
	, m_scenatio_file(scenario_file_name)
{
	if(m_scenatio_file.fail()){
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
	
}

/* private関数 */

/*
	アクションを記述した文字列1行をトークンに分割する関数
	引数1: const std::string& action_msg アクションを記述した文字列
	返り値: std::vector<std::string> 分割の結果得られたトークン
*/
std::vector<std::string> CScenarioReader::split_action_message(const std::string& action_msg)
{
	
}