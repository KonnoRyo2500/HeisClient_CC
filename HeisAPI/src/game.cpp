// heis ゲーム進行管理クラス
// Author: Ryo Konno

#include "game.h"

/* public関数 */

/*
	heis ゲーム実行メイン処理
	引数なし
	返り値なし
*/
void CGame::play_game()
{
	bool battle_result;

	// 対戦開始前の準備
	prepare_to_battle();

	// 対戦
	while (!is_battle_end()) {
		// ユーザAIの行動
	}

	// 勝敗判定
	battle_result = judge_win();
	// 勝敗を表示
	printf("%s", battle_result ? "You win!" : "You lose...");
}

/* private関数 */

/*
	対戦を開始する前の準備を行う関数
	引数なし
	返り値なし
*/
void CGame::prepare_to_battle()
{
	// メッセージ(名前要求)を受信
	recv_name_request();
	// 名前を送信
	name_entry("test");
	// 確定した名前を受信
	name_register();
}

/*
	サーバーから，名前要求を受信する関数
	引数なし
	返り値なし
*/
void CGame::recv_name_request()
{
	// TODO: サーバーからメッセージを受け取り，名前要求かどうか判定する
	// 名前要求でなければエラー
}

/*
	指定された名前をサーバーに送る関数
	引数1: const std::string& name 名前
	返り値なし
*/
void CGame::name_entry(const std::string& name)
{
	m_team_name = name;
	// TODO: m_team_nameをサーバーに送信
}

/*
	サーバーから受信した名前をチーム名として登録する関数
	引数なし
	返り値なし
*/
void CGame::name_register()
{
	// TODO: サーバーから名前を受信し，それをm_team_nameに入れる
}

/*
	対戦が終了したか判定する関数
	引数なし
	返り値: bool 対戦が終了しているか
*/
bool CGame::is_battle_end()
{
	// TODO: サーバーから受信したJSONの"finished"フィールドを返す
	return true;
}

/*
	対戦の決着がついた後，フィールドの状態から勝敗を決定する関数
	引数なし
	返り値: bool 勝敗(true: 自チームの勝ち, false: 自チームの負け)
*/
bool CGame::judge_win()
{
	// TODO: フィールドを参照し，自チームの兵士が一人でもいたらtrue, 一人もいなかったらfalseを返す
	return true;
}