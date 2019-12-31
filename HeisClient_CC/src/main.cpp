// heis メイン処理
// Author: Ryo Konno
#include "game_local.h"

int main()
{
	int exit_code = EXIT_SUCCESS;
	printf("Hello heis!\n");

	try {
		CGameLocal game;

		game.play_game();
	}
	catch (const std::exception& e) {
		fprintf(stderr, "heisの対戦中にエラーが発生しました(内容: %s)\n", e.what());
		exit_code = EXIT_FAILURE;
	}
	return exit_code;
}