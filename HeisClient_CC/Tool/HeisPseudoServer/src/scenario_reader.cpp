// heisクライアント用JSON送受信治具 シナリオ読み込みクラス
// Author: Ryo Konno

#include "scenario_reader.h"
#include "heis_client_exception.h"

/*
	コンストラクタ
	引数1: const std::string& scenario_file_name シナリオファイル名
	例外: シナリオファイルのオープンに失敗したとき
*/
CScenarioReader::CScenarioReader(const std::string& scenario_file_name)
	: m_next_action_type(ActionType_AllActionDone)
{
	m_scenatio_file = fopen(scenario_file_name.c_str(), "r");
	if(!m_scenatio_file){
		throw CHeisClientException("シナリオファイルのオープンに失敗しました(ファイル名: %s)", scenario_file_name.c_str());
	}
}

/*
	デストラクタ
	引数なし
*/
CScenarioReader::~CScenarioReader()
{
	fclose(m_scenatio_file);
	m_scenatio_file = NULL;
}


