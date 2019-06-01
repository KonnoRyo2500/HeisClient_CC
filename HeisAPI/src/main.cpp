// heis メイン処理
// Author: Ryo Konno
#include <cstdio>
#include <cstdlib>
//#include "field.h"
#include "socket.h"
#include "game_local.h"

int main()
{
	int exit_code = EXIT_SUCCESS;
	printf("Hello heis!\n");

	try {
		CSocket sck("192.168.1.102", 8823);
		sck.send_to("test message");
		printf("recvmsg: %s\n", sck.recv_from().c_str());
		//CGameLocal game;

		//game.play_game();
	}
	catch (const std::exception & e) {
		printf("heisの対戦中にエラーが発生しました(内容: %s)\n", e.what());
		exit_code = EXIT_FAILURE;
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
	return exit_code;
}