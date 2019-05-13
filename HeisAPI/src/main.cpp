// heis メイン処理
// Author: Ryo Konno
#include <cstdio>
#include <cstdlib>
#include "field.h"

int main()
{
	printf("Hello heis!\n");

	CField::create_field();

	CInfantry infantry("test", 0, 0, 0);
	CInfantry enemy_infantry("test_enemy", 0, 1, 0);

	infantry.attack(Direction_Left);

	CField::delete_field();

	return EXIT_SUCCESS;
}