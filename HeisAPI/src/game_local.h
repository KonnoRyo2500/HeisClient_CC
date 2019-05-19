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

	// メンバ関数
	public:
		void play_game();

	private:
		// 対戦開始前処理
		void prepare_to_battle();

		// 大戦終了後処理
		void cleanup_after_battle();

		// 対戦の終了判定
		bool is_battle_end();

		// 対戦後の勝敗判定
		bool judge_win();

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