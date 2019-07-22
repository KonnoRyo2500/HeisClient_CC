// heis ゲーム(ローカルモード)進行管理クラス
// Author: Ryo Konno
#pragma once

#include "commander.h"
#include "AI.h"
#include "JSON_analyzer.h"
#include "pseudo_server.h"
#include <string>

class CGameLocal {
	// 構造体，列挙体など
	private:
		// 現在のターンに関する情報
		enum CurrentTurn {
			CurrentTurn_MyTurn,				// 自チームのターン
			CurrentTurn_EnemyTurn,				// 敵チームのターン
		};

		// 対戦結果
		enum BattleResult {
			BattleResult_MyTeamWin,			// 自チームの勝利
			BattleResult_EnemyTeamWin,		// 敵チームの勝利
			BattleResult_Remain,			// 対戦が続行中
			// 注) ゲームの仕様上，引き分けはありえないはず
		};

	// メンバ関数
	public:
		void play_game();

	private:
		// 対戦開始前処理
		void prepare_to_battle();

		// 対戦中処理
		void turn_entry() const;
		void turn_action(const CurrentTurn current_turn) const;
		void turn_end(const CurrentTurn current_turn) const;
		void switch_current_turn(CurrentTurn& current_turn) const;

		// 対戦終了後処理
		void cleanup_after_battle();

		// 対戦結果の判定
		BattleResult judge_battle_result() const;

	// メンバ変数
	private:
		// 兵士を操作するための司令官
		CCommander* m_my_commander;
		CCommander* m_enemy_commander;

		// ユーザAI
		CUserAI* m_my_AI;
		CUserAI* m_enemy_AI;

		// JSON解析器
		CJSONAnalyzer* m_json_analyzer;

		// 疑似サーバ
		CPseudoServer* m_pseudo_server;
		
};