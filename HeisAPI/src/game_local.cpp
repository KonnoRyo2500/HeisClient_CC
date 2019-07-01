// heis ゲーム(ローカルモード)進行管理クラス
// Author: Ryo Konno

#include "game_local.h"
#include "field.h"

/* public関数 */

/*
	heis ゲーム(ローカルモード)実行メイン処理
	引数なし
	返り値なし
*/
void CGameLocal::play_game()
{
	BattleResult battle_result;

	// 対戦開始前の準備
	prepare_to_battle();

	// 対戦
	while ((battle_result = judge_battle_result()) == BattleResult_Remain) {
		// ターン開始時処理
		turn_entry();

		// ユーザAIの行動
		m_my_AI->AI_main();
		m_enemy_AI->AI_main();

		CField::get_instance()->show();
	}

	// 対戦終了後処理
	cleanup_after_battle();
	// 勝敗を表示
	printf("%s\n", battle_result == BattleResult_MyTeamWin ? "You win!" : "You lose...");
}

/* private関数 */

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
	m_enemy_AI = new CUserAI(m_enemy_commander);
}

/*
	ターン開始時の処理を行う関数
	引数なし
	返り値なし
*/
void CGameLocal::turn_entry()
{
	m_my_commander->update();
	m_enemy_commander->update();
}

/*
	対戦終了後の後処理を行う関数
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
	対戦結果を判定する関数
	引数なし
	返り値: BattleResult 対戦結果
*/
CGameLocal::BattleResult CGameLocal::judge_battle_result()
{
	CField* field = CField::get_instance();
	bool my_team_alive = false;
	bool enemy_team_alive = false;

	// フィールド上のすべての兵士を探索し，少なくとも一方のチームの兵士が全滅していたら終了
	for (int x = 0; x < FieldParam_Width; x++) {
		for (int y = 0; y < FieldParam_Height; y++) {
			CInfantry* infantry = field->get_infantry(x, y);

			if (infantry != NULL) {
				if (infantry->get_team_name() == m_my_team_name) {
					my_team_alive = true;
				}
				else if (infantry->get_team_name() == m_enemy_team_name) {
					enemy_team_alive = true;
				}
			}
		}
	}

	BattleResult result = BattleResult_Remain;

	if (!my_team_alive) {
		result = BattleResult_EnemyTeamWin;
	}
	else if (!enemy_team_alive) {
		result = BattleResult_MyTeamWin;
	}

	return result;
}