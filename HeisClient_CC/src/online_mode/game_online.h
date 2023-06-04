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
#include "client_socket.h"
#include "setting.h"
#include <string>
#include <vector>

/**
*	@brief	オンラインモード実行クラス
*/
class CGameOnline : public CGame {
	// メンバ関数
	public:
		// 対戦開始
		void play_game() override;
		
	private:
		// 対戦開始前処理
		void initialize_battle(OnlineSetting setting);
		void recv_name_request() const;
		void name_entry(const std::string& name);
		void name_register(OnlineSetting setting);

		// 対戦終了後処理
		void finalize_battle();

		// 対戦後の勝敗判定
		bool judge_win();

	// メンバ変数
	private:
		//! 自チーム名
		std::string m_team_name;

		//! 司令官
		CCommander* m_commander;

		//! AI
		CAIBase* m_ai;

		//! TCPソケット
		CClientSocket* m_sck;
};