// heis メイン処理
// Author: Ryo Konno
#include <cstdio>
#include <cstdlib>
#include "JSON_analyzer.h"
#include "game_local.h"

// デバッグ用関数
static CCommander::JSONSendData_Action create_sample_action_pkt();
static CGameOnline::JSONSendData_Name create_sample_name_pkt();

static std::string create_sample_name_decided_JSON();

static void show_name_decided_pkt(const CGameOnline::JSONRecvData_NameDecided& name_decided_pkt);


int main()
{
	int exit_code = EXIT_SUCCESS;
	printf("Hello heis!\n");

	try {
#if 1
		CJSONAnalyzer json_analyzer;
		printf("「行動」JSON: %s\n", json_analyzer.create_action_JSON(create_sample_action_pkt()).c_str());
		printf("「名前」JSON: %s\n", json_analyzer.create_name_JSON(create_sample_name_pkt()).c_str());
		show_name_decided_pkt(json_analyzer.create_name_decided_pkt(create_sample_name_decided_JSON()));
#else
		CGameLocal game;

		game.play_game();
#endif
	}
	catch (const std::exception & e) {
		printf("heisの対戦中にエラーが発生しました(内容: %s)\n", e.what());
		exit_code = EXIT_FAILURE;
	}
	return exit_code;
}

// 「行動」パケットを作成(デバッグ用)
static CCommander::JSONSendData_Action create_sample_action_pkt()
{
	CCommander::JSONSendData_Action sample_action_pkt;

	sample_action_pkt.turn_team = "test";
	for (int i = 1; i <= 5; i++) {
		CInfantry::JSONSendData_Content content;

		content.atk_x = 1 * i;
		content.atk_y = 2 * i;
		content.to_x = 3 * i;
		content.to_y = 4 * i;
		{
			char sample_id[10] = {0};
			sprintf_s(sample_id, "sample%d", i);
			content.unit_id = std::string(sample_id);
		}
		sample_action_pkt.contents.push_back(content);
	}

	return sample_action_pkt;
}

// 「名前」パケットを作成(デバッグ用)
static CGameOnline::JSONSendData_Name create_sample_name_pkt()
{
	CGameOnline::JSONSendData_Name sample_name_pkt;

	sample_name_pkt.team_name = "sample";

	return sample_name_pkt;
}

// 「名前確定」JSONを作成(デバッグ用)
static std::string create_sample_name_decided_JSON()
{
	return "{ \"your_team\": \"foo\" }";
}

// 「名前確定」パケットの中身を表示(デバッグ用)
static void show_name_decided_pkt(const CGameOnline::JSONRecvData_NameDecided& name_decided_pkt)
{
	printf("「名前確定」パケット:\n");
	printf("\tyour_team: %s\n", name_decided_pkt.your_team.c_str());
}