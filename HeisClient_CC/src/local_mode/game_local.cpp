// heis ゲーム(ローカルモード)進行管理クラス
// Author: Ryo Konno

#include "game_local.h"
#include "field.h"
#include "setting_keys.h"

#define LOCAL_SETTING_FILE_NAME "local_setting.csv"

/* public関数 */

/*
	heis ゲーム(ローカルモード)実行メイン処理
	引数なし
	返り値なし
*/
void CGameLocal::play_game()
{
	// 対戦の初期化
	initialize_battle();

	// 対戦終了処理
	finalize_battle();

}

/* private関数 */

/*
	対戦を開始する前の準備を行う関数
	引数なし
	返り値なし
*/
void CGameLocal::initialize_battle()
{
	// 設定ファイルの読み込み
	load_local_mode_setting();

	// フィールドの生成
	CField::create_field();

	// 各インスタンスの生成
	m_my_commander = new CCommander(m_setting.my_team_name);
	m_enemy_commander = new CCommander(m_setting.enemy_team_name);

	m_my_AI = new CUserAI(m_my_commander);
	m_enemy_AI = new COpponentAI(m_enemy_commander);

	m_pseudo_server = new CPseudoServer();

	m_json_analyzer = new CJSONAnalyzer();
}

/*
	対戦終了後の後処理を行う関数
	引数なし
	返り値なし
*/
void CGameLocal::finalize_battle()
{
	// フィールドを削除
	CField::delete_field();

	// メンバ変数のメモリ解放
	delete m_my_commander;
	delete m_enemy_commander;
	delete m_my_AI;
	delete m_enemy_AI;
	delete m_pseudo_server;
	delete m_json_analyzer;
	// m_settingについては実体を保持しているため，明示的なメモリ解放は必要ない

	m_my_commander = NULL;
	m_enemy_commander = NULL;
	m_my_AI = NULL;
	m_enemy_AI = NULL;
	m_pseudo_server = NULL;
	m_json_analyzer = NULL;
}

/*
	ローカルモード設定ファイルから，設定を取得する関数
	引数なし
	返り値なし
*/
void CGameLocal::load_local_mode_setting()
{
	CCsvSettingFileReader local_setting_file(LOCAL_SETTING_FILE_NAME);

	m_setting.field_width = local_setting_file.get_single_value<uint16_t>(LOCAL_SETTING_KEY_FIELD_WIDTH, 0);
	m_setting.field_height = local_setting_file.get_single_value<uint16_t>(LOCAL_SETTING_KEY_FIELD_HEIGHT, 0);
	m_setting.my_team_name = local_setting_file.get_single_value<std::string>(LOCAL_SETTING_KEY_MY_NAME, 0);
	m_setting.enemy_team_name = local_setting_file.get_single_value<std::string>(LOCAL_SETTING_KEY_ENEMY_NAME, 0);
	m_setting.my_team_init_pos = get_initial_position(local_setting_file, LOCAL_SETTING_KEY_MY_INIT_POS);
	m_setting.enemy_team_init_pos = get_initial_position(local_setting_file, LOCAL_SETTING_KEY_ENEMY_INIT_POS);
	m_setting.is_my_team_first =
		(local_setting_file.get_single_value<std::string>(LOCAL_SETTING_KEY_FIRST_TEAM, 0) == m_setting.my_team_name);
}

/*
	兵士の初期位置をローカルモード設定ファイルから取得する関数
	引数1: const CCsvSettingFileReader& local_setting_file ローカルモード設定ファイル
	引数2: const std::string& key 初期位置を取得するためのキー
	返り値: std::vector<FieldPosition> 初期位置
*/
std::vector<FieldPosition>  CGameLocal::get_initial_position(const CCsvSettingFileReader& local_setting_file, const std::string& key)
{
	// 初期位置は"x1 y1,x2 y2,..."の形式で記載されているものとする
	std::vector<std::string> all_init_pos_str = local_setting_file.get_all_value<std::string>(key);
	std::vector<FieldPosition> init_pos;

	for (auto& pos_str : all_init_pos_str) {
		CTokenManager tm;
		FieldPosition pos;
		token_array_t pos_token;

		pos_token = tm.split_string(pos_str, " ");
		if (pos_token.size() != 2) {
			throw CHeisClientException("兵士の初期位置の指定が不正です");
		}
		pos.x = stoi(tm.get_single_token(pos_token, 0));
		pos.y = stoi(tm.get_single_token(pos_token, 1));
		init_pos.push_back(pos);
	}

	return init_pos;
}