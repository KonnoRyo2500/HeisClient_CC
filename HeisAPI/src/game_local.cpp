// heis ゲーム(ローカルモード)進行管理クラス
// Author: Ryo Konno

#include "game_local.h"
#include "field.h"

/*
	heis ゲーム(ローカルモード)実行メイン処理
	引数なし
	返り値なし
*/
void CGameLocal::play_game()
{
	bool battle_result;

	// 対戦開始前の準備
	prepare_to_battle();

	// 対戦
	while (!is_battle_end()) {
		// ユーザAIの行動
	}

	// 大戦終了後処理
	cleanup_after_battle();
	// 勝敗判定
	battle_result = judge_win();
	// 勝敗を表示
	printf("%s\n", battle_result ? "You win!" : "You lose...");
}

/*
	対戦を開始する前の準備を行う関数
	引数なし
	返り値なし
*/
void CGameLocal::prepare_to_battle()
{
	const bool is_my_team_bottom_left = true;		// 自軍を左下に配置するか

	// 必要なインスタンスの生成
	CField::create_field();

	// 兵士の初期配置はここで行われる
	m_my_commander = new CCommander(m_my_team_name, InitAreaParam_Width, InitAreaParam_Height, is_my_team_bottom_left);
	m_enemy_commander = new CCommander(m_enemy_team_name, InitAreaParam_Width, InitAreaParam_Height, !is_my_team_bottom_left);

	m_my_AI = new CUserAI(m_my_commander);
	m_enemy_AI = new CUserAI(m_my_commander);
}

/*
	大戦終了後の後処理を行う関数
	引数なし
	返り値なし
*/
void CGameLocal::cleanup_after_battle()
{
	delete m_my_commander;
	delete m_enemy_commander;
	delete m_my_AI;
	delete m_enemy_AI;

	m_my_commander = NULL;
	m_enemy_commander = NULL;
	m_my_AI = NULL;
	m_enemy_AI = NULL;

	CField::delete_field();
}

/*
	対戦が終了したか判定する関数
	引数なし
	返り値: bool 対戦が終了しているか
*/
bool CGameLocal::is_battle_end()
{
	return true;
}

/*
	対戦の決着がついた後，フィールドの状態から勝敗を決定する関数
	引数なし
	返り値: bool 勝敗(true: 自チームの勝ち, false: 自チームの負け)
*/
bool CGameLocal::judge_win()
{
	return true;
}