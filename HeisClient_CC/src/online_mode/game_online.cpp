/**
*	@file		game_online.cpp
*	@brief		heis ゲーム(オンラインモード)進行管理クラス
*	@author		Ryo Konno
*	@details	オンラインモードでheisのゲームを実行する．
*/
#include "game_online.h"
#include "const_val.h"
#include "field.h"
#include "setting_keys.h"
#include "path_generator.h"

/**
*	@def ONLINE_SETTING_FILE_NAME
*	@brief オンラインモード設定ファイルの名前
*/
#define ONLINE_SETTING_FILE_NAME "online_setting.csv"

/* public関数 */

/**
*	@brief heis ゲーム実行メイン処理
*/
void CGameOnline::play_game()
{
	g_system_log->write_log(CLog::LogType_Infomation, false, "オンラインモードでゲームを開始しました");
	g_battle_log->write_log(CLog::LogType_Infomation, false, "オンラインモードでゲームを開始しました");

	bool battle_result;

	// 対戦の準備
	initialize_battle();

	recv_name_request();
	name_entry(m_setting_file->get_single_value<std::string>(ONLINE_SETTING_KEY_TEAM_NAME, 0));
	name_register();

	// 対戦
	while (true) {
		// サーバから受信するJSONから生成するパケット(「盤面」パケットはここで生成できる)
		JSONRecvPacket_Field field_pkt = m_json_analyzer->create_field_pkt(m_sck->sck_recv());
		JSONRecvPacket_Result result_pkt;

		// 受信した「盤面」JSONの内容に合うよう，内部のフィールドを更新
		CField::get_instance()->update(field_pkt);
		m_commander->update();

		// 盤面を表示
		CField::get_instance()->show();

		// 「盤面」パケットは一旦変数に持っておきたいため，while文の条件部で対戦終了の判定をしない
		if (field_pkt.finished) {
			break;
		}
		// 自分のターンでなければ，次の「盤面」JSON受信まで待つ
		if (field_pkt.turn_team != m_team_name) {
			continue;
		}

		// ユーザAIの行動
		m_ai->AI_main(field_pkt);

		// 「行動」パケットを作成して送信
		m_sck->sck_send(m_json_analyzer->create_action_JSON(m_commander->create_action_pkt()));

		// 「結果」パケットを受信
		result_pkt = m_json_analyzer->create_result_pkt(m_sck->sck_recv());
		// 「結果」パケットの内容を表示
		for (const auto& result_elem : result_pkt.result) {
			g_system_log->write_log(CLog::LogType_Warning, true, "サーバからエラーメッセージが送信されました(エラー内容: %s, 対象兵士ID: %s)", 
				result_elem.error.c_str(), 
				result_elem.unit_id.is_omitted() ? "なし" : result_elem.unit_id.get().c_str());
		}
	}

	// 対戦終了
	battle_result = judge_win();
	finalize_battle();

	// 勝敗を表示
	g_battle_log->write_log(CLog::LogType_Infomation, true, battle_result ? "You win!" : "You lose...");
	g_system_log->write_log(CLog::LogType_Infomation, false, "ゲームが終了しました");
}

/* private関数 */

/**
*	@brief 対戦を開始する前の準備を行う関数
*	@remark この関数では，名前確定前に生成できるインスタンスを生成する
*/
void CGameOnline::initialize_battle()
{
	// 必要なインスタンスの生成
	CField::create_field();

	m_setting_file = new CCsvSettingFileReader(get_setting_dir() + ONLINE_SETTING_FILE_NAME);
	// m_commander, m_aiの生成については，名前確定後に行う必要があるため，name_register関数で行う
	m_json_analyzer = new CJSONAnalyzer();
	m_sck = new CClientSocket();

	const std::string svr_addr = m_setting_file->get_single_value<std::string>(ONLINE_SETTING_KEY_SVR_ADDR, 0);
	const uint16_t svr_port = m_setting_file->get_single_value<uint16_t>(ONLINE_SETTING_KEY_SVR_PORT, 0);
	g_system_log->write_log(CLog::LogType_Infomation, false, "サーバに接続します(IPアドレス: %s, ポート番号: %d)",
		svr_addr.c_str(), svr_port);

	// サーバに接続
	m_sck->sck_connect(svr_addr, svr_port);

	g_system_log->write_log(CLog::LogType_Infomation, false, "サーバに接続しました(IPアドレス: %s, ポート番号: %d)",
		svr_addr.c_str(), svr_port);
	g_system_log->write_log(CLog::LogType_Infomation, false, "インスタンスの生成が完了しました");
}

/**
*	@brief サーバーから，名前要求を受信する関数
*/
void CGameOnline::recv_name_request() const
{
	std::string received_JSON = m_sck->sck_recv();
	JSONRecvPacket_Message name_req_msg_pkt = m_json_analyzer->create_message_pkt(received_JSON);
}

/**
*	@brief 指定された名前をサーバーに送る関数
*	@param[in] name 名前
*/
void CGameOnline::name_entry(const std::string& name)
{
	JSONSendPacket_Name name_pkt = {name};
	m_sck->sck_send(m_json_analyzer->create_name_JSON(name_pkt));
	g_system_log->write_log(CLog::LogType_Infomation, false, "チーム名をサーバに送信しました(チーム名: %s)",
		name.c_str());
}

/**
*	@brief サーバーから受信した名前をチーム名として登録する関数
*/
void CGameOnline::name_register()
{
	std::string received_JSON = m_sck->sck_recv();
	JSONRecvPacket_NameDecided name_decided_pkt = m_json_analyzer->create_name_decided_pkt(received_JSON);

	m_team_name = name_decided_pkt.your_team;
	m_commander = new CCommander(m_team_name);
	m_ai = new CUserAI(m_commander);

	g_system_log->write_log(CLog::LogType_Infomation, false, "チーム名が確定しました(チーム名: %s)",
		m_team_name.c_str());
}

/**
*	@brief 対戦終了後の後処理を行う関数
*/
void CGameOnline::finalize_battle()
{
	delete m_commander;
	delete m_ai;
	delete m_json_analyzer;
	delete m_sck;
	delete m_setting_file;

	m_commander = NULL;
	m_ai = NULL;
	m_json_analyzer = NULL;
	m_sck = NULL;
	m_setting_file = NULL;

	CField::delete_field();

	g_system_log->write_log(CLog::LogType_Infomation, false, "インスタンスの削除が完了しました");
}

/**
*	@brief 対戦の決着がついた後，フィールドの状態から勝敗を決定する関数
*	@return bool 勝敗(true: 自チームの勝ち, false: 自チームの負け)
*/
bool CGameOnline::judge_win()
{
	// 最終状態のフィールドを司令官インスタンスに反映する
	m_commander->update();
	// 自チームが勝っていれば，敵の兵士はいないので，少なくとも1人の兵士は移動できる
	return m_commander->get_all_actable_infantry_ids(m_team_name).size() > 0;
}