// heis メイン処理
// Author: Ryo Konno
#include <cstdio>
#include <cstdlib>
#include "JSON_analyzer.h"
#include "game_local.h"

// デバッグ用関数
static JSONSendPacket_Action create_sample_action_pkt();
static JSONSendPacket_Name create_sample_name_pkt();

static std::string create_sample_name_decided_JSON();
static std::string create_sample_message_JSON();
static std::string create_sample_result_JSON();
static std::string create_sample_field_JSON();

static void show_name_decided_pkt(const JSONRecvPacket_NameDecided& name_decided_pkt);
static void show_message_pkt(const JSONRecvPacket_Message& message_pkt);
static void show_result_pkt(const JSONRecvPacket_Result& result_pkt);
static void show_field_pkt(const JSONRecvPacket_Field& field_pkt);

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
		show_message_pkt(json_analyzer.create_message_pkt(create_sample_message_JSON()));
		show_result_pkt(json_analyzer.create_result_pkt(create_sample_result_JSON()));
		show_field_pkt(json_analyzer.create_field_pkt(create_sample_field_JSON()));
#else
		CGameLocal game;

		game.play_game();
#endif
	}
	catch (const std::exception& e) {
		fprintf(stderr, "heisの対戦中にエラーが発生しました(内容: %s)\n", e.what());
		exit_code = EXIT_FAILURE;
	}
	return exit_code;
}

// 「行動」パケットを作成(デバッグ用)
static JSONSendPacket_Action create_sample_action_pkt()
{
	JSONSendPacket_Action sample_action_pkt;

	sample_action_pkt.turn_team = "test";
	for (int i = 1; i <= 5; i++) {
		ContentsArrayElem content;

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
static JSONSendPacket_Name create_sample_name_pkt()
{
	JSONSendPacket_Name sample_name_pkt;

	sample_name_pkt.team_name = "sample";

	return sample_name_pkt;
}

// 「名前確定」JSONを作成(デバッグ用)
static std::string create_sample_name_decided_JSON()
{
	return "{ \"your_team\": \"foo\" }";
}

// 「メッセージ」JSONを作成(デバッグ用)
static std::string create_sample_message_JSON()
{
	return "{\"message\": \"test_message\"}";
}

// 「結果」JSONを作成(デバッグ用)
static std::string create_sample_result_JSON()
{
	std::string json("{");
	const int num_elem = 10;

	json += "\"result\":[";
	for (int i = 0; i < num_elem; i++) {
		json += "{";
		json += "\"unit_id\":\"XXX\",";
		json += "\"error\": \"test_error_msg\"";
		json += (i != num_elem - 1 ? "}," : "}");
	}
	json += "]}";
	return json;
}

// 「盤面」JSONを作成(デバッグ用)
static std::string create_sample_field_JSON()
{
	std::string json("{");
	const int num_players = 3;
	const int num_units = 5;

	json += "\"width\":20,";
	json += "\"height\":20,";
	json += "\"turn_team\":\"X\",";
	json += "\"count\":5,";
	json += "\"finished\":true,";
	json += "\"players\":[";
	for (int i = 0; i < num_players; i++) {
		json += "\"XXX\"";
		json += (i != num_players - 1 ? "," : "],");
	}
	json += "\"units\":[";
	for (int i = 0; i < num_units; i++) {
		json += "{";
		json += "\"type\":\"heizu\",";
		json += "\"unit_id\":\"YYY\",";
		json += "\"team\":\"sample_team\",";
		json += "\"hp\": 100,";
		json += "\"locate\":{\"x\":10,\"y\":20}";
		json += "}";
		json += (i != num_units - 1 ? "," : "");
	}
	json += "]";
	json += "}";

	return json;
}

// 「名前確定」パケットの中身を表示(デバッグ用)
static void show_name_decided_pkt(const JSONRecvPacket_NameDecided& name_decided_pkt)
{
	printf("「名前確定」パケット:\n");
	printf("\tyour_team: %s\n", name_decided_pkt.your_team.c_str());
}

// 「メッセージ」パケットの中身を表示(デバッグ用)
static void show_message_pkt(const JSONRecvPacket_Message& message_pkt)
{
	printf("「メッセージ」パケット:\n");
	printf("\tmessage: %s\n", message_pkt.message.c_str());
}

// 「結果」パケットの中身を表示(デバッグ用)
static void show_result_pkt(const JSONRecvPacket_Result& result_pkt)
{
	printf("「結果」パケット\n");
	int i = 1;
	printf("\tresult:\n");
	for (auto& result_elem : result_pkt.result) {
		printf("\t\t(要素%d)\n", i);
		if (!result_elem.unit_id.is_omitted()) {
			printf("\t\t\tunit_id: %s\n", result_elem.unit_id.get().c_str());
		}
		printf("\t\t\terror: %s\n", result_elem.error.c_str());
		i++;
	}
}

// 「盤面」パケットの中身を表示(デバッグ用)
static void show_field_pkt(const JSONRecvPacket_Field& field_pkt)
{
	int i;

	printf("「盤面」パケット\n");
	printf("\twidth: %d\n", field_pkt.width);
	printf("\theight: %d\n", field_pkt.height);
	printf("\tturn_team: %s\n", field_pkt.turn_team.c_str());
	printf("\tcount: %d\n", field_pkt.count);
	printf("\tfinished: %d\n", field_pkt.finished);
	printf("\tplayers:\n");
	i = 1;
	for (auto& player : field_pkt.players) {
		printf("\t\t(要素%d)\n", i);
		printf("\t\t\t%s\n", player.c_str());
		i++;
	}
	i = 1;
	printf("\tunits:\n");
	for (auto& unit : field_pkt.units) {
		printf("\t\t(要素%d)\n", i);
		printf("\t\t\ttype: %s\n", unit.type.c_str());
		printf("\t\t\tunit_id: %s\n", unit.unit_id.c_str());
		printf("\t\t\thp: %d\n", unit.hp);
		printf("\t\t\tteam: %s\n", unit.team.c_str());
		printf("\t\t\tlocate: (%d, %d)\n", unit.locate.x, unit.locate.y);
		i++;
	}

}