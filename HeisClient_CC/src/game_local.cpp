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
	CurrentTurn turn = CurrentTurn_MyTurn;
	do {
		// ターン開始時処理
		turn_entry();

		// ユーザAIの行動
		if (turn == CurrentTurn_MyTurn) {
			m_my_AI->AI_main();
			m_pseudo_server->recv_action_json(m_json_analyzer->create_action_JSON(m_my_commander->create_action_pkt()));
		}
		else {
			m_enemy_AI->AI_main();
			m_pseudo_server->recv_action_json(m_json_analyzer->create_action_JSON(m_enemy_commander->create_action_pkt()));
		}

		CField::get_instance()->show();
		switch_current_turn(turn);
	} while ((battle_result = judge_battle_result()) == BattleResult_Remain);

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
	// 必要なインスタンスの生成
	CField::create_field();

	// 兵士の初期配置はここで行われる
	m_my_commander = new CCommander(m_my_team_name);
	m_enemy_commander = new CCommander(m_enemy_team_name);

	m_my_AI = new CUserAI(m_my_commander);
	m_enemy_AI = new CUserAI(m_enemy_commander);

	m_json_analyzer = new CJSONAnalyzer();

	m_pseudo_server = new CPseudoServer();
}

/*
	ターン開始時の処理を行う関数
	引数なし
	返り値なし
*/
void CGameLocal::turn_entry() const
{
	bool is_first_turn = (CField::get_instance()->get_width() == 0 || CField::get_instance()->get_height() == 0);
	JSONRecvPacket_Field field_pkt;

	// フィールドの更新
	// 最初のターンは，初期配置を記した「盤面」JSONを受け取る
	if (is_first_turn) {
		field_pkt = m_json_analyzer->create_field_pkt(m_pseudo_server->send_initial_field_json());
	}
	// それ以降のターンは，フィールドの状態をそのまま反映した「盤面」JSONを受け取る
	else {
		field_pkt = m_json_analyzer->create_field_pkt(m_pseudo_server->send_field_json());
	}
	CField::get_instance()->update(field_pkt);

	// 各司令官の持つ兵士情報を，最新のフィールド状態に合うよう更新
	m_my_commander->update();
	m_enemy_commander->update();
}

/*
	ユーザAI行動時の処理を行う関数
	引数1: const CurrentTurn current_turn 現在のターン
	返り値なし
*/
void CGameLocal::turn_action(const CurrentTurn current_turn) const
{

}

/*
	ターン終了時の処理を行う関数
	引数1: const CurrentTurn current_turn 現在のターン
	返り値なし
*/
void CGameLocal::turn_end(const CurrentTurn current_turn) const
{

}

/*
	現在のターンを切り替える関数
	参照1: CurrentTurn& current_turn 現在のターン
	返り値なし
*/
void CGameLocal::switch_current_turn(CurrentTurn& current_turn) const
{
	if (current_turn == CurrentTurn_MyTurn) {
		current_turn = CurrentTurn_EnemyTurn;
	}
	else {
		current_turn = CurrentTurn_MyTurn;
	}
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
	delete m_json_analyzer;
	delete m_pseudo_server;

	m_my_commander = NULL;
	m_enemy_commander = NULL;
	m_my_AI = NULL;
	m_enemy_AI = NULL;
	m_json_analyzer = NULL;
	m_pseudo_server = NULL;

	CField::delete_field();
}

/*
	対戦結果を判定する関数
	引数なし
	返り値: BattleResult 対戦結果
*/
CGameLocal::BattleResult CGameLocal::judge_battle_result() const
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