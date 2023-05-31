/**
*	@file		game_local.h
*	@brief		heis ゲーム(ローカルモード)進行管理クラス
*	@author		Ryo Konno
*	@details	ローカルモードでheisのゲームを実行する．
*/
#pragma once

#include "game.h"
#include "commander.h"
#include "ai_base.h"
#include "pseudo_server.h"
#include "local_setting.h"
#include "setting_file_reader.h"
#include "log.h"

#include <string>

extern CLog *g_system_log;
extern CLog *g_battle_log;

/**
*	@brief	ローカルモード実行クラス
*/
class CGameLocal : public CGame {
	// 構造体，列挙体など
	private:

	// メンバ関数
	public:
		void play_game() override;

	private:
		// 設定を取得する
		void load_local_mode_setting();
		void load_initial_position(const CSettingFileReader& reader);

		// 対戦の初期化
		void initialize_battle();

		// 対戦の終了処理
		void finalize_battle();

		// 対戦後の勝敗判定
		bool judge_win() override;

	// メンバ変数
	private:

		//! 自分の司令官
		CCommander* m_my_commander;
		//! 敵の司令官
		CCommander* m_enemy_commander;

		//! 自分のAI
		CAIBase* m_my_AI;
		//! 敵のAI
		CAIBase* m_enemy_AI;

		//! 疑似サーバ
		CPseudoServer* m_pseudo_server;
		
		//! ローカルモード設定
		LocalSetting m_setting;
};