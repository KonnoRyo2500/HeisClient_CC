// heis メイン処理
// Author: Ryo Konno
#include "game_local.h"
#include "game_online.h"
#include "log.h"

/* グローバル変数 */

/* 各種ログ */
// ログファイル格納先のディレクトリは存在している前提
// Gitにコミットされているので，通常使用する際はディレクトリが存在しているはず

// フィールドログ
CLog g_field_log("field_log");
// 対戦ログ
CLog g_battle_log("battle_log");
// システムログ
CLog g_system_log("system_log");


// ゲームモード
enum GameMode {
	// ローカルモード
	GameMode_Local,
	// オンラインモード
	GameMode_Online,
};

/* static関数 */

// ゲームモードを選択する
static GameMode ask_game_mode();
// ゲームを開始する
static void start_game(const GameMode mode);

int main()
{
	int exit_code = EXIT_SUCCESS;
	printf("Hello heis!\n");

	try {
		start_game(ask_game_mode());
	}
	catch (const std::exception& e) {
		fprintf(stderr, "heisの対戦中にエラーが発生しました(内容: %s)\n", e.what());
		exit_code = EXIT_FAILURE;
	}
	return exit_code;
}

/*
	ユーザにゲームモードを選択させる関数
	引数なし
	返り値: GameMode 選択したゲームモード
*/
static GameMode ask_game_mode()
{
	// ユーザが選択した値とゲームモードとの対応表
	static const std::map<int, GameMode> selected_value_to_mode = {
		{1, GameMode_Local},
		{2, GameMode_Online},
	};
	auto it = selected_value_to_mode.end();

	while (it == selected_value_to_mode.end()) {
		// ゲームモードが確定するまで選択を続行する
		try {
			std::string user_input_buf;
			printf("ゲームモードを入力して下さい(1: ローカルモード, 2: オンラインモード)\n");
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

/*
	指定したモードでゲームを開始する関数
	引数1: const GameMode mode ゲームモード
	返り値なし
*/
static void start_game(const GameMode mode)
{
	// TODO: ゲームクラス(CGameLocal, CGameOnline)の親となるクラスを作成すれば，この関数の処理を改善できそう
	CGameLocal game_local = CGameLocal();
	CGameOnline game_online = CGameOnline();

	switch (mode) {
		case GameMode_Local:
			game_local.play_game();
			break;
		case GameMode_Online:
			game_online.play_game();
			break;
		default:
			throw CHeisClientException("ゲームモードが不正です");
	}
}