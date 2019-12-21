// heis ゲーム(ローカルモード)進行管理クラス
// Author: Ryo Konno
#pragma once

#include "commander.h"
#include "user_AI.h"
#include "opponent_AI.h"
#include "JSON_analyzer.h"
#include "pseudo_server.h"
#include "local_setting.h"
#include "csv_setting_file_reader.h"

#include <string>

class CGameLocal {
	// 構造体，列挙体など
	private:

	// メンバ関数
	public:
		void play_game();

	private:
		// 設定を取得する
		void load_local_mode_setting();
		std::vector<FieldPosition> get_initial_position(const CCsvSettingFileReader& local_setting_file, const std::string& key);	

		// 対戦の初期化
		void initialize_battle();

		// 対戦の終了処理
		void finalize_battle();

	// メンバ変数
	private:

		// 司令官
		CCommander* m_my_commander;
		CCommander* m_enemy_commander;

		// AI
		CUserAI* m_my_AI;
		COpponentAI* m_enemy_AI;

		// 疑似サーバ
		CPseudoServer* m_pseudo_server;

		// JSON解析
		CJSONAnalyzer* m_json_analyzer;
		
		// 設定
		LocalSetting m_setting;
};