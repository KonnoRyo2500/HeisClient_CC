// heisクライアント用疑似サーバ メイン処理
// Author: Ryo Konno

#include "scenario_reader.h"

/*
	メイン関数
*/
int main(int argc, char **argv)
{
	try{
		CScenarioReader sr("scenario_sample.txt");
		sr.get_next_aciton_type();
	}
	catch(std::exception& e){
		printf("exception: %s\n", e.what());
	}
	return EXIT_SUCCESS;
}