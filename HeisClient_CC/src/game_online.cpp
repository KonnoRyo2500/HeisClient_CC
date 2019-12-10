// heis ゲーム(オンラインモード)進行管理クラス
// Author: Ryo Konno

#include "game_online.h"
#include "const_val.h"
#include "field.h"
#include "setting_keys.h"

#define ONLINE_SETTING_FILE_NAME "online_setting.csv"

/* public関数 */

/*
	heis ゲーム実行メイン処理
	引数なし
	返り値なし
*/
void CGameOnline::play_game()
{
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

		// 自分のターン

		// ユーザAIの行動
		m_ai->AI_main();

		// 「行動」パケットを作成して送信
		m_sck->sck_send(m_json_analyzer->create_action_JSON(m_commander->create_action_pkt()));

		// 「結果」パケットを受信
		result_pkt = m_json_analyzer->create_result_pkt(m_sck->sck_recv());
		// 「結果」パケットの内容を表示
		// TODO: 「結果」パケットの内容を，ログファイルに記録できるようにする
		for (const auto& result_elem : result_pkt.result) {
			printf("エラー内容: %s, 対象兵士ID: %s\n", result_elem.error.c_str(), result_elem.unit_id.is_omitted() ? "なし" : result_elem.unit_id.get().c_str());
		}
	}

	// 対戦終了
	battle_result = judge_win();
	finalize_battle();

	// 勝敗を表示
	printf("%s\n", battle_result ? "You win!" : "You lose...");
}

/* private関数 */

/*
	対戦を開始する前の準備を行う関数
	引数なし
	返り値なし
	備考: この関数では，名前確定前に生成できるインスタンスを生成する
*/
void CGameOnline::initialize_battle()
{
	// 必要なインスタンスの生成
	CField::create_field();

	m_setting_file = new CCsvSettingFileReader(ONLINE_SETTING_FILE_NAME);
	// m_commander, m_aiの生成については，名前確定後に行う必要があるため，name_register関数で行う
	m_json_analyzer = new CJSONAnalyzer();
	m_sck = new CClientSocket();
	m_sck->sck_connect(m_setting_file->get_single_value<std::string>(ONLINE_SETTING_KEY_SVR_ADDR, 0), 
					   m_setting_file->get_single_value<uint16_t>(ONLINE_SETTING_KEY_SVR_PORT, 0));
}

/*
	サーバーから，名前要求を受信する関数
	引数なし
	返り値なし
*/
void CGameOnline::recv_name_request() const
{
	std::string received_JSON = m_sck->sck_recv();
	JSONRecvPacket_Message name_req_msg_pkt = m_json_analyzer->create_message_pkt(received_JSON);
}

/*
	指定された名前をサーバーに送る関数
	引数1: const std::string& name 名前
	返り値なし
*/
void CGameOnline::name_entry(const std::string& name)
{
	JSONSendPacket_Name name_pkt = {name};
	m_sck->sck_send(m_json_analyzer->create_name_JSON(name_pkt));
}

/*
	サーバーから受信した名前をチーム名として登録する関数
	引数なし
	返り値なし
*/
void CGameOnline::name_register()
{
	std::string received_JSON = m_sck->sck_recv();
	JSONRecvPacket_NameDecided name_decided_pkt = m_json_analyzer->create_name_decided_pkt(received_JSON);

	m_team_name = name_decided_pkt.your_team;
	m_commander = new CCommander(m_team_name);
	m_ai = new CUserAI(m_commander);
}

/*
	対戦終了後の後処理を行う関数
	引数なし
	返り値なし
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
}

/*
	対戦の決着がついた後，フィールドの状態から勝敗を決定する関数
	引数なし
	返り値: bool 勝敗(true: 自チームの勝ち, false: 自チームの負け)
*/
bool CGameOnline::judge_win()
{
	// TODO: フィールドを参照し，自チームの兵士が一人でもいたらtrue, 一人もいなかったらfalseを返す
	return true;
}