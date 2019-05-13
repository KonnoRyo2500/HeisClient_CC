// heis メイン処理
// Author: Ryo Konno
#include <cstdio>
#include <cstdlib>
#include "field.h"

int main()
{
	printf("Hello heis!\n");

	CField::create_field();

	CField* field = CField::get_instance();
	CInfantry infantry("test", 0, 0, 0);
	CInfantry enemy_infantry("test_enemy", 0, 1, 0);

	field->set_infantry(0, 15, &infantry);
	field->set_infantry(0, 0, NULL);

	infantry.update_status();

	printf("infantry is in (%d, %d)\n", infantry.get_x_position(), infantry.get_y_position());

	CField::delete_field();

	return EXIT_SUCCESS;
}