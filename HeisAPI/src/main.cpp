// heis メイン処理
// Author: Ryo Konno
#include <cstdio>
#include <cstdlib>
//#include "field.h"
#include "game_online.h"

int main()
{
	printf("Hello heis!\n");

	try {
		CGameOnline game;

		game.play_game();
	}
	catch (const std::exception & e) {
		printf("heisの対戦中にエラーが発生しました(内容: %s)\n", e.what());
	}

#if 0
	CField::create_field();

	CField* field = CField::get_instance();
	CInfantry infantry("test", "te01", 2, 1);
	CInfantry enemy_infantry("test_enemy", "en01", 1, 1);

	for (auto v : enemy_infantry.look_around()) {
		if (v.infantry != NULL) {
			printf("direction: %d, ID: %s\n", v.direction, v.infantry->get_id().c_str());
		}
		else {
			printf("direction: %d, ID: NULL\n", v.direction);
		}
	}

	CField::delete_field();
#endif
	return EXIT_SUCCESS;
}