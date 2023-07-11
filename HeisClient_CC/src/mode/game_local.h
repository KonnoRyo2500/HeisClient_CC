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
#include "log.h"
#include "setting.h"

#include <string>

/**
*	@brief	ローカルモード実行クラス
*/
class CGameLocal : public CGame {
	// メンバ関数
public:
		void play_game() override;

private:
		// 最初のターンの「盤面」パケットを作成する
		JSONRecvPacket_Board create_initial_board_packet(const LocalSetting& setting) const;
		// 最初の「盤面」パケットの"units"要素を作成する
		std::vector<UnitsArrayElem> create_units_of_initial_board_packet(const LocalSetting& setting) const;

		// 盤面上の兵士の行動回数をリセットする
		void reset_infantry_action_remain(CBoard& board) const;

		// 次のターンのチーム名を取得する
		std::string get_next_turn_team_name(const JSONRecvPacket_Board& pkt, const LocalSetting& setting) const;
		// 勝利しているチーム名を取得する
		std::string get_winning_team_name(const CBoard& board) const;
};