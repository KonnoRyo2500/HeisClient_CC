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

/* グローバル変数 */

/* 各種ログ */
// ログファイル格納先のディレクトリは存在している前提
// Gitにコミットされているので，通常使用する際はディレクトリが存在しているはず

//! フィールドログ
CLog g_field_log("field_log");
//! 対戦ログ
CLog g_battle_log("battle_log");
//! システムログ
CLog g_system_log("system_log");


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
	int exit_code = EXIT_SUCCESS;

	try {
		g_system_log.write_log(CLog::LogType_Infomation, false, "CCの実行が開始されました");
		start_game(ask_game_mode());
		g_system_log.write_log(CLog::LogType_Infomation, false, "CCの実行が正常に完了しました");
	}
	catch (const std::exception& e) {
		g_system_log.write_log(CLog::LogType_Error, true, "heisの対戦中にエラーが発生しました(内容: %s)\n", e.what());
		exit_code = EXIT_FAILURE;
	}
	return exit_code;
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
		catch (std::exception e) {
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
	// TODO: ゲームクラス(CGameLocal, CGameOnline)の親となるクラスを作成すれば，この関数の処理を改善できそう
	CGameLocal game_local = CGameLocal();
	CGameOnline game_online = CGameOnline();
	CAudienceMode audience = CAudienceMode();

	switch (mode) {
		case GameMode_Local:
			game_local.play_game();
			break;
		case GameMode_Online:
			game_online.play_game();
			break;
		case GameMode_Audience:
			audience.watch_game();
			break;
		default:
			throw CHeisClientException("ゲームモードが不正です");
	}
}