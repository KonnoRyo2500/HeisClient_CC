// heis ゲーム(ローカルモード)進行管理クラス
// Author: Ryo Konno
#pragma once

#include "commander.h"
#include "AI.h"
#include <string>

class CGameLocal {
	// 構造体，列挙体など
	private:
		// 初期化領域のサイズ
		enum InitAreaParam {
			InitAreaParam_Width = 6,		// 幅
			InitAreaParam_Height = 4,		// 高さ
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

		// 対戦中処理(ターン開始)
		void turn_entry();

		// 対戦終了後処理
		void cleanup_after_battle();

		// 対戦結果の判定
		BattleResult judge_battle_result();

	// メンバ変数
	private:

		// 注意: 最初の2文字が被らないような名前にしておくこと(被るとIDが衝突し，動作が未定義になる)
		const std::string m_my_team_name = "test";
		const std::string m_enemy_team_name = "enemy";

		CCommander* m_my_commander;
		CCommander* m_enemy_commander;

		CUserAI* m_my_AI;
		CUserAI* m_enemy_AI;
};