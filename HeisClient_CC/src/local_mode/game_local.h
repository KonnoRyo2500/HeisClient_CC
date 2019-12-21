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

	// メンバ変数
	private:
		
		// 設定
		LocalSetting m_setting;
};