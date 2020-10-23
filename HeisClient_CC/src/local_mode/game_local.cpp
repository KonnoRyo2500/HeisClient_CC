/**
*	@file		game_local.cpp
*	@brief		heis ゲーム(ローカルモード)進行管理クラス
*	@author		Ryo Konno
*	@details	ローカルモードでheisのゲームを実行する．
*/

#include "game_local.h"
#include "board.h"
#include "setting_keys.h"
#include "common.h"

/**
*	@def LOCAL_SETTING_FILE_NAME
*	@brief ローカルモード設定ファイルの名前
*/
#define LOCAL_SETTING_FILE_NAME "local_setting.csv"

/* public関数 */

/**
*	@brief ローカルモード実行メイン処理
*/
void CGameLocal::play_game()
{
	bool battle_result;

	g_system_log->write_log(CLog::LogLevel_InvisibleInfo, "ローカルモードでゲームを開始しました");
	g_battle_log->write_log(CLog::LogLevel_InvisibleInfo, "ローカルモードでゲームを開始しました");

	// 対戦の初期化
	initialize_battle();

	// 対戦
	while (true) {
		// オンラインモードと同様に，疑似サーバから受け取った「盤面」JSONを基に盤面を更新してから行動する
		// しかし，「結果」JSONの送信など，不要な処理は行わない
		JSONRecvPacket_Board board_pkt = m_json_analyzer->create_board_pkt(m_pseudo_server->send_board_json(m_setting));

		// 盤面更新
		CBoard::get_instance()->update(board_pkt);

		// 盤面を表示
		CBoard::get_instance()->show();

		if (board_pkt.finished) {
			break;
		}

		if (board_pkt.turn_team == m_setting.my_team_name) {
			// 自チームのターン
			m_my_commander->update();
			m_my_AI->AI_main(board_pkt);
		}
		else if(board_pkt.turn_team == m_setting.enemy_team_name) {
			// 敵チームのターン
			m_enemy_commander->update();
			m_enemy_AI->AI_main(board_pkt);
		}
		else {
			throw std::runtime_error(cc_common::format("不正なチーム名が「盤面」JSONの\"turn_team\"フィールドに設定されています(チーム名: %s)"
				,board_pkt.turn_team.c_str()));
		}
	}

	// 勝敗を判定
	battle_result = judge_win();

	// 勝敗を表示
	g_battle_log->write_log(CLog::LogLevel_VisibleInfo, battle_result ? "You win!" : "You lose...");

	// 対戦終了処理
	finalize_battle();

	g_system_log->write_log(CLog::LogLevel_InvisibleInfo, "ゲームが終了しました");
}

/* private関数 */

/**
*	@brief 対戦を開始する前の準備を行う関数
*/
void CGameLocal::initialize_battle()
{
	// 設定ファイルの読み込み
	load_local_mode_setting();

	// 盤面の生成
	CBoard::create_board();

	// 各インスタンスの生成
	m_my_commander = new CCommander(m_setting.my_team_name);
	m_enemy_commander = new CCommander(m_setting.enemy_team_name);

	m_my_AI = new CUserAI(m_my_commander);
	m_enemy_AI = new COpponentAI(m_enemy_commander);

	m_pseudo_server = new CPseudoServer();

	m_json_analyzer = new CJSONAnalyzer();

	g_system_log->write_log(CLog::LogLevel_InvisibleInfo, "インスタンスの生成が完了しました");
}

/**
*	@brief 対戦終了後の後処理を行う関数
*/
void CGameLocal::finalize_battle()
{
	// 盤面を削除
	CBoard::delete_board();

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

	g_system_log->write_log(CLog::LogLevel_InvisibleInfo, "インスタンスの削除が完了しました");
}

/**
*	@brief ローカルモード設定ファイルから，設定を取得する関数
*/
void CGameLocal::load_local_mode_setting()
{
	CSettingFileReader local_setting_file(
		cc_common::get_setting_dir()
		+ cc_common::get_separator_char()
		+ LOCAL_SETTING_FILE_NAME
	);

	m_setting.board_width = local_setting_file.get_single_value<uint16_t>(LOCAL_SETTING_KEY_BOARD_WIDTH, 0);
	m_setting.board_height = local_setting_file.get_single_value<uint16_t>(LOCAL_SETTING_KEY_BOARD_HEIGHT, 0);
	m_setting.my_team_name = local_setting_file.get_single_value<std::string>(LOCAL_SETTING_KEY_MY_NAME, 0);
	m_setting.enemy_team_name = local_setting_file.get_single_value<std::string>(LOCAL_SETTING_KEY_ENEMY_NAME, 0);
	m_setting.my_team_init_pos = get_initial_position(local_setting_file, LOCAL_SETTING_KEY_MY_INIT_POS);
	m_setting.enemy_team_init_pos = get_initial_position(local_setting_file, LOCAL_SETTING_KEY_ENEMY_INIT_POS);
	m_setting.is_my_team_first =
		(local_setting_file.get_single_value<std::string>(LOCAL_SETTING_KEY_FIRST_TEAM, 0) == m_setting.my_team_name);
	g_system_log->write_log(CLog::LogLevel_InvisibleInfo, "ローカルモード設定ファイルの読み込みが完了しました");
}

/**
*	@brief 兵士の初期位置をローカルモード設定ファイルから取得する関数
*	@param[in] local_setting_file ローカルモード設定ファイル
*	@param[in] key 初期位置を取得するためのキー
*	@return std::vector<BoardPosition> 初期位置
*/
std::vector<BoardPosition>  CGameLocal::get_initial_position(const CSettingFileReader& local_setting_file, const std::string& key)
{
	// 初期位置は"x1 y1,x2 y2,..."の形式で記載されているものとする
	std::vector<std::string> all_init_pos_str = local_setting_file.get_all_value<std::string>(key);
	std::vector<BoardPosition> init_pos;

	for (auto& pos_str : all_init_pos_str) {
		BoardPosition pos;
		std::vector<std::string> pos_token;

		pos_token = cc_common::split_string(pos_str, " ");
		if (pos_token.size() != 2) {
			throw std::runtime_error("兵士の初期位置の指定が不正です");
		}
		pos.x = stoi(pos_token[0]);
		pos.y = stoi(pos_token[1]);
		init_pos.push_back(pos);
	}

	return init_pos;
}

/**
*	@brief 対戦の決着がついた後，勝敗を決定する関数
*	@return bool 勝敗(true: 自チームの勝ち, false: 自チームの負け)
*/
bool CGameLocal::judge_win()
{
	// 最終状態の盤面を司令官インスタンスに反映する
	m_my_commander->update();
	// 自チームが勝っていれば，敵の兵士はいないので，少なくとも1人の兵士は移動できる
	return m_my_commander->get_all_actable_infantry_ids(m_setting.my_team_name).size() > 0;
}