﻿// heis ゲーム(オンラインモード)進行管理クラス
// Author: Ryo Konno
#pragma once

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
		void prepare_to_battle();

		// 対戦開始前処理の実行関数
		void recv_name_request();
		void name_entry(const std::string& name);
		void name_register();

		// 対戦の終了判定
		bool is_battle_end();

		// 対戦後の勝敗判定
		bool judge_win();

	// メンバ変数
	private:
		// 自チーム名
		std::string m_team_name;
};