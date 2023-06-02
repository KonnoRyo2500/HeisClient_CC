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
#include "ai_factory.h"
#include "board_json_converter.h"
#include "local_setting_file.h"

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

	// 設定ファイルの読み込み
	LocalSetting setting = CLocalSettingFile().load(
		cc_common::get_setting_dir()
		+ cc_common::get_separator_char()
		+ LOCAL_SETTING_FILE_NAME
	);

	// 対戦の初期化
	initialize_battle(setting);

	// 対戦
	while (true) {
		// オンラインモードと同様に，疑似サーバから受け取った「盤面」JSONを基に盤面を更新してから行動する
		// しかし，「結果」JSONの送信など，不要な処理は行わない
		BoardJsonConverter board_json_converter;
		JSONRecvPacket_Board board_pkt = board_json_converter.from_json_to_packet(m_pseudo_server->send_board_json(setting));

		// 盤面更新
		CBoard::get_instance()->update(board_pkt);

		// 盤面を表示
		CBoard::get_instance()->show();

		if (board_pkt.finished.get_value()) {
			break;
		}

		if (board_pkt.turn_team.get_value() == setting.my_team_name) {
			// 自チームのターン
			m_my_commander->update();
			m_my_AI->AI_main(board_pkt);
		}
		else if(board_pkt.turn_team.get_value() == setting.enemy_team_name) {
			// 敵チームのターン
			m_enemy_commander->update();
			m_enemy_AI->AI_main(board_pkt);
		}
		else {
			throw std::runtime_error(cc_common::format("不正なチーム名が「盤面」JSONの\"turn_team\"フィールドに設定されています(チーム名: %s)"
				,board_pkt.turn_team.get_value().c_str()));
		}
	}

	// 勝敗を判定
	battle_result = judge_win(setting);

	// 勝敗を表示
	g_battle_log->write_log(CLog::LogLevel_VisibleInfo, battle_result ? "You win!" : "You lose...");

	// 対戦終了処理
	finalize_battle();

	g_system_log->write_log(CLog::LogLevel_InvisibleInfo, "ゲームが終了しました");
}

/* private関数 */

/**
*	@brief 対戦を初期化する
*	@param[in] setting ローカルモード設定値
*/
void CGameLocal::initialize_battle(LocalSetting setting)
{
	// 盤面の生成
	CBoard::create_board();

	// 各インスタンスの生成
	m_my_commander = new CCommander(setting.my_team_name);
	m_enemy_commander = new CCommander(setting.enemy_team_name);

	CAIFactory ai_factory = CAIFactory();
	m_my_AI = ai_factory.create_instance(m_my_commander, setting.my_team_ai_impl);
	m_enemy_AI = ai_factory.create_instance(m_enemy_commander, setting.enemy_team_ai_impl);
	if (m_my_AI == NULL){
		throw std::runtime_error("自チームのAIインスタンス生成に失敗しました。AI実装の設定をご確認ください");
	}
	if (m_enemy_AI == NULL) {
		throw std::runtime_error("敵チームのAIインスタンス生成に失敗しました。AI実装の設定をご確認ください");
	}

	m_pseudo_server = new CPseudoServer();

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

	m_my_commander = NULL;
	m_enemy_commander = NULL;
	m_my_AI = NULL;
	m_enemy_AI = NULL;
	m_pseudo_server = NULL;

	g_system_log->write_log(CLog::LogLevel_InvisibleInfo, "インスタンスの削除が完了しました");
}

/**
*	@brief 対戦の決着がついた後，勝敗を決定する関数
*	@param[in] setting ローカルモード設定値
*	@return bool 自チームが勝ったか
*/
bool CGameLocal::judge_win(LocalSetting setting) const
{
	// 最終状態の盤面を司令官インスタンスに反映する
	m_my_commander->update();
	// 自チームが勝っていれば，敵の兵士はいないので，少なくとも1人の兵士は移動できる
	return m_my_commander->get_all_actable_infantry_ids(setting.my_team_name).size() > 0;
}