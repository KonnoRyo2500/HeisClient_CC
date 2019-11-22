// heis ゲーム(オンラインモード)進行管理クラス
// Author: Ryo Konno
#pragma once

#include "commander.h"
#include "AI.h"
#include "JSON_analyzer.h"
#include "client_socket.h"
#include <string>
#include <vector>

class CGameOnline {
	// 構造体，列挙体など
	public:

	// メンバ関数
	public:
		// 対戦開始
		void play_game();
		
	private:
		// 対戦開始前処理
		void initialize_battle();
		void recv_name_request() const;
		void name_entry(const std::string& name);
		void name_register();

		// 対戦終了後処理
		void finalize_battle();

		// 対戦後の勝敗判定
		bool judge_win();

	// メンバ変数
	private:
		// 自チーム名
		std::string m_team_name;

		// 司令官
		CCommander* m_commander;

		// ユーザAI
		CUserAI* m_ai;
		
		// JSON解析器
		CJSONAnalyzer* m_json_analyzer;

		// TCPソケット
		CClientSocket* m_sck;
};