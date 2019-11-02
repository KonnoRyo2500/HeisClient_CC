// heis ゲーム(オンラインモード)進行管理クラス
// Author: Ryo Konno

#include "game_online.h"
#include "const_val.h"
#include "field.h"

/* public関数 */

/*
	heis ゲーム実行メイン処理
	引数なし
	返り値なし
*/
void CGameOnline::play_game()
{
	bool battle_result;

	// 対戦開始前の準備
	initialize_battle();
	// メッセージ(名前要求)を受信
	recv_name_request();
	// 名前を送信
	// TODO: サーバに送信する名前を，設定ファイルから取得できるようにする
	name_entry(LOCAL_MY_TEAM_NAME);
	// 確定した名前を受信
	name_register();

	// 対戦
	while (!is_battle_end()) {
		// ユーザAIの行動
	}

	// 勝敗判定
	battle_result = judge_win();
	// 対戦終了後処理
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

	// 兵士の初期配置はここで行われる
	// m_commanderの生成については，名前確定後に行う必要があるため，name_register関数で行う
	m_ai = new CUserAI(m_commander);
	m_json_analyzer = new CJSONAnalyzer();
	// TODO: 設定ファイルから接続先IPアドレス，ポート番号を決定できるようにする
	m_sck = new CSocket();
	m_sck->sck_connect("192.168.1.1", 50000);
}

/*
	サーバーから，名前要求を受信する関数
	引数なし
	返り値なし
*/
void CGameOnline::recv_name_request()
{
	JSONRecvPacket_Message name_req_msg_pkt = m_json_analyzer->create_message_pkt(m_sck->sck_recv());
	printf("名前要求: %s\n", name_req_msg_pkt.message.c_str());
	// 名前要求でなければエラー
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
	JSONRecvPacket_NameDecided name_decided_pkt = m_json_analyzer->create_name_decided_pkt(m_sck->sck_recv());
	m_team_name = name_decided_pkt.your_team;
	m_commander = new CCommander(m_team_name);
}

/*
	対戦が終了したか判定する関数
	引数なし
	返り値: bool 対戦が終了しているか
*/
bool CGameOnline::is_battle_end()
{
	// TODO: サーバーから受信したJSONの"finished"フィールドを返す
	return true;
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

	m_commander = NULL;
	m_ai = NULL;
	m_json_analyzer = NULL;
	m_sck = NULL;

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