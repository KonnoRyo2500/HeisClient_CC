/**
*	@file		game_online.h
*	@brief		heis ゲーム(オンラインモード)進行管理クラス
*	@author		Ryo Konno
*	@details	オンラインモードでheisのゲームを実行する．
*/
#pragma once

#include "game.h"
#include "commander.h"
#include "ai_base.h"
#include "JSON_analyzer.h"
#include "client_socket.h"
#include "setting_file_reader.h"
#include <string>
#include <vector>

extern CLog *g_system_log;
extern CLog *g_battle_log;

/**
*	@brief	オンラインモード実行クラス
*/
class CGameOnline : public CGame {
	// 構造体，列挙体など
	public:

	// メンバ関数
	public:
		// 対戦開始
		void play_game() override;
		
	private:
		// 対戦開始前処理
		void initialize_battle();
		void recv_name_request() const;
		void name_entry(const std::string& name);
		void name_register();

		// 対戦終了後処理
		void finalize_battle();

		// 対戦後の勝敗判定
		bool judge_win() override;

	// メンバ変数
	private:
		//! 自チーム名
		std::string m_team_name;

		//! 司令官
		CCommander* m_commander;

		//! AI
		CAIBase* m_ai;
		
		//! JSON解析器
		CJSONAnalyzer* m_json_analyzer;

		//! TCPソケット
		CClientSocket* m_sck;

		//! オンラインモード設定ファイル
		CSettingFileReader* m_setting_file;
};