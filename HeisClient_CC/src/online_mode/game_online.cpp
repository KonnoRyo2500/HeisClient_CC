/**
*	@file		game_online.cpp
*	@brief		heis ゲーム(オンラインモード)進行管理クラス
*	@author		Ryo Konno
*	@details	オンラインモードでheisのゲームを実行する．
*/
#include "game_online.h"
#include "const_val.h"
#include "board.h"
#include "setting_keys.h"
#include "ai_factory.h"
#include "board_json_converter.h"
#include "action_json_converter.h"
#include "result_json_converter.h"
#include "message_json_converter.h"
#include "name_json_converter.h"
#include "confirm_name_json_converter.h"
#include "online_setting_file.h"
#include "path.h"

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
	CLog::write(CLog::LogLevel_Information, "オンラインモードでゲームを開始しました");
	CLog::write(CLog::LogLevel_Information, "オンラインモードでゲームを開始しました");

	bool battle_result;

	// 設定ファイルの読み込み
	OnlineSetting setting = COnlineSettingFile().load(
		join({CC_SETTING_DIR, ONLINE_SETTING_FILE_NAME})
	);

	// 対戦の準備
	initialize_battle(setting);

	recv_name_request();
	name_entry(setting.team_name);
	name_register(setting);

	// 対戦
	while (true) {
		// サーバから受信するJSONから生成するパケット(「盤面」パケットはここで生成できる)
		BoardJsonConverter board_json_converter;
		ActionJsonConverter action_json_converter;
		ResultJsonConverter result_json_converter;
		JSONRecvPacket_Board board_pkt = board_json_converter.from_json_to_packet(m_sck->sck_recv());
		JSONRecvPacket_Result result_pkt;

		// 受信した「盤面」JSONの内容に合うよう，内部のフィールドを更新
		CBoard::get_instance()->update(board_pkt);
		m_commander->update();

		// 盤面を表示
		CBoard::get_instance()->show();

		// 「盤面」パケットは一旦変数に持っておきたいため，while文の条件部で対戦終了の判定をしない
		if (board_pkt.finished.get_value()) {
			break;
		}
		// 自分のターンでなければ，次の「盤面」JSON受信まで待つ
		if (board_pkt.turn_team.get_value() != m_team_name) {
			continue;
		}

		// ユーザAIの行動
		m_ai->AI_main(board_pkt);

		// 「行動」パケットを作成して送信
		m_sck->sck_send(action_json_converter.from_packet_to_json(m_commander->create_action_pkt()));

		// 「結果」パケットを受信
		result_pkt = result_json_converter.from_json_to_packet(m_sck->sck_recv());
		// 「結果」パケットの内容を表示
		for (const auto& result_elem : result_pkt.result.get_value()) {
			CLog::write(CLog::LogLevel_Warning,cc_common::format(
				"サーバからエラーメッセージが送信されました(エラー内容: %s, 対象兵士ID: %s)",
				result_elem.error.get_value().c_str(), 
				result_elem.unit_id.exists() ? result_elem.unit_id.get_value().c_str() : "なし"));
		}
	}

	// 対戦終了
	battle_result = judge_win();
	finalize_battle();

	// 勝敗を表示
	CLog::write(CLog::LogLevel_Information, battle_result ? "You win!" : "You lose...", true);
	CLog::write(CLog::LogLevel_Information, "ゲームが終了しました");
}

/* private関数 */

/**
*	@brief 対戦を開始する前の準備を行う関数
*	@param[in] setting オンラインモード設定値
*	@remark この関数では，名前確定前に生成できるインスタンスを生成する
*/
void CGameOnline::initialize_battle(OnlineSetting setting)
{
	// 必要なインスタンスの生成
	CBoard::create_board();

	// m_commander, m_aiの生成については，名前確定後に行う必要があるため，name_register関数で行う
	m_sck = new CClientSocket();
	CLog::write(CLog::LogLevel_Information, cc_common::format(
		"サーバに接続します(IPアドレス: %s, ポート番号: %d)",
		setting.server_ip_addr.c_str(), setting.server_port_num));

	// サーバに接続
	m_sck->sck_connect(setting.server_ip_addr, setting.server_port_num);

	CLog::write(CLog::LogLevel_Information, cc_common::format(
		"サーバに接続しました(IPアドレス: %s, ポート番号: %d)",
		setting.server_ip_addr.c_str(), setting.server_port_num));
	CLog::write(CLog::LogLevel_Information, "インスタンスの生成が完了しました");
}

/**
*	@brief サーバーから，名前要求を受信する関数
*/
void CGameOnline::recv_name_request() const
{
	std::string received_JSON = m_sck->sck_recv();
	MessageJsonConverter message_json_converter;
	JSONRecvPacket_Message name_req_msg_pkt = message_json_converter.from_json_to_packet(received_JSON);
}

/**
*	@brief 指定された名前をサーバーに送る関数
*	@param[in] name 名前
*/
void CGameOnline::name_entry(const std::string& name)
{
	JSONSendPacket_Name name_pkt;
	NameJsonConverter name_json_converter;
	name_pkt.team_name.set_value(name);
	m_sck->sck_send(name_json_converter.from_packet_to_json(name_pkt));
	CLog::write(CLog::LogLevel_Information, cc_common::format(
		"チーム名をサーバに送信しました(チーム名: %s)",
		name.c_str()));
}

/**
*	@brief サーバーから受信した名前をチーム名として登録する関数
*	@param[in] setting オンラインモード設定値
*/
void CGameOnline::name_register(OnlineSetting setting)
{
	std::string received_JSON = m_sck->sck_recv();
	ConfirmNameJsonConverter confirm_name_json_converter;
	JSONRecvPacket_ConfirmName confirm_name_pkt = confirm_name_json_converter.from_json_to_packet(received_JSON);
	CAIFactory ai_factory = CAIFactory();

	m_team_name = confirm_name_pkt.your_team.get_value();
	m_commander = new CCommander(m_team_name);
	m_ai = ai_factory.create_instance(
		m_commander,
		setting.ai_impl
	);
	if (m_ai == NULL) {
		throw std::runtime_error("AIインスタンス生成に失敗しました。AI実装の設定をご確認ください");
	}

	CLog::write(CLog::LogLevel_Information, cc_common::format(
		"チーム名が確定しました(チーム名: %s)",
		m_team_name.c_str()));
}

/**
*	@brief 対戦終了後の後処理を行う関数
*/
void CGameOnline::finalize_battle()
{
	delete m_commander;
	delete m_ai;
	delete m_sck;

	m_commander = NULL;
	m_ai = NULL;
	m_sck = NULL;

	CBoard::delete_board();

	CLog::write(CLog::LogLevel_Information, "インスタンスの削除が完了しました");
}

/**
*	@brief 対戦の決着がついた後，盤面の状態から勝敗を決定する関数
*	@return bool 勝敗(true: 自チームの勝ち, false: 自チームの負け)
*/
bool CGameOnline::judge_win()
{
	// 最終状態の盤面を司令官インスタンスに反映する
	m_commander->update();
	// 自チームが勝っていれば，敵の兵士はいないので，少なくとも1人の兵士は移動できる
	return m_commander->get_all_actable_infantry_ids(m_team_name).size() > 0;
}