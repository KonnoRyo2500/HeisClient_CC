// heis ゲーム(ローカルモード)進行管理クラス
// Author: Ryo Konno
#pragma once

#include <string>

class CGameLocal {
	// 構造体，列挙体など

	// メンバ関数
public:
	void play_game();

private:
	// 対戦開始前処理
	void prepare_to_battle();

	// 対戦開始前処理の実行関数
	void positioning_infantry();
	std::string make_id(const std::string& prefix, const int16_t number);

	// 対戦の終了判定
	bool is_battle_end();

	// 対戦後の勝敗判定
	bool judge_win();

	// メンバ変数
	const std::string m_team_name = "test";
	const std::string m_enemy_name = "enemy";
};