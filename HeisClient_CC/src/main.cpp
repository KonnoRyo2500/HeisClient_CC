/**
*	@file		main.cpp
*	@brief		heis メイン処理
*	@author		Ryo Konno
*	@details	メイン関数を定義するファイル．
*/

#include "game_local.h"
#include "game_online.h"
#include "audience_mode.h"
#include "log.h"

#include <map>
#include <iostream>

/**
*	@enum GameMode
*	ゲームモード
*/
enum GameMode {
	//! ローカルモード
	GameMode_Local,
	//! オンラインモード
	GameMode_Online,
	//! 観戦モード
	GameMode_Audience,
};

/* static関数 */
//! ゲームモードを選択する
static GameMode ask_game_mode();
//! ゲームを開始する
static void start_game(const GameMode mode);

/**
*	@brief メイン関数
*/
int main()
{
	try {
		// 各種ログファイルを作成する
		CLog::start_logging();

		// 対戦を実行
		CLog::write(CLog::LogLevel_Information, "CCの実行が開始されました");
		start_game(ask_game_mode());
		CLog::write(CLog::LogLevel_Information, "CCの実行が正常に完了しました");

		// ログの記録を終了
		CLog::end_logging();
	}
	catch (const std::exception& e) {
		// ログのインスタンスが正常に作成できていない可能性もあるため、コンソール出力にする
		fprintf(stderr, "CCの実行中にエラーが発生しました(内容: %s)\n", e.what());
		return EXIT_FAILURE;
	}

	return EXIT_SUCCESS;
}

/**
*	@brief ユーザにゲームモードを選択させる関数
*	@return GameMode 選択したゲームモード
*/
static GameMode ask_game_mode()
{
	// ユーザが選択した値とゲームモードとの対応表
	static const std::map<int, GameMode> selected_value_to_mode = {
		{1, GameMode_Local},
		{2, GameMode_Online},
		{3, GameMode_Audience},
	};
	auto it = selected_value_to_mode.end();

	while (it == selected_value_to_mode.end()) {
		// ゲームモードが確定するまで選択を続行する
		try {
			std::string user_input_buf;
			printf("ゲームモードを入力して下さい(1: ローカルモード, 2: オンラインモード, 3: 観戦モード)\n");
			std::cin >> user_input_buf;
			it = selected_value_to_mode.find(stoi(user_input_buf));
		}
		catch (const std::exception& e) {
			printf("入力されたゲームモードは受理できません(理由: %s)\n", e.what());
			continue;
		}
	}

	return it->second;
}

/**
*	@brief 指定したモードでゲームを開始する関数
*	@param[in] mode ゲームモード
*/
static void start_game(const GameMode mode)
{
	CGame* game;

	switch (mode) {
		case GameMode_Local:
			game = new CGameLocal();
			break;
		case GameMode_Online:
			game = new CGameOnline();
			break;
		case GameMode_Audience:
			game = new CAudienceMode();
			break;
		default:
			throw std::runtime_error("ゲームモードが不正です");
	}
	game->play_game();

	delete game;
	game = NULL;
}