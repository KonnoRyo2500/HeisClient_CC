// heisクライアント用疑似サーバ メイン処理
// Author: Ryo Konno

#include "scenario_reader.h"
#include "JSON_sender.h"
#include "JSON_receiver.h"
#include "server_socket.h"
#include "csv_setting_file_reader.h"
#include "ps_setting_keys.h"
#include "path_generator.h"

#define SETTING_FILE_NAME "heis_ps_setting.csv"

/*
	メイン関数
*/
int main(int argc, char **argv)
{
	try{
		CPathGenerator pg;
		CCsvSettingFileReader setting_reader(pg.get_exe_path() + SETTING_FILE_NAME);
		const std::string scenario_file_name = setting_reader.get_single_value<std::string>(PS_SETTING_FILE_KEY_SCENARIO_NAME, 0);
		CScenarioReader scenario_reader(scenario_file_name);
		CScenarioReader::ActionType act_type;
		CJsonSender json_sender;
		CJsonReceiver json_reader;
		CServerSocket next_com_sck;
		CServerSocket com_sck_to_first, com_sck_to_second;
		
		// ソケットの初期化
		com_sck_to_first.sck_bind(setting_reader.get_single_value<uint16_t>(PS_SETTING_FILE_KEY_LISTEN_PORT, 0),
								  setting_reader.get_single_value<std::string>(PS_SETTING_FILE_KEY_LISTEN_ADDR, 0));
		com_sck_to_first.sck_listen();
		com_sck_to_first.sck_accept();
		
		com_sck_to_second.sck_bind(setting_reader.get_single_value<uint16_t>(PS_SETTING_FILE_KEY_LISTEN_PORT, 1),
								   setting_reader.get_single_value<std::string>(PS_SETTING_FILE_KEY_LISTEN_ADDR, 1));
		com_sck_to_second.sck_listen();
		com_sck_to_second.sck_accept();
		
		while((act_type = scenario_reader.get_next_aciton_type()) != CScenarioReader::ActionType_AllActionDone){
			printf("type: %d\n", act_type);
			next_com_sck = (scenario_reader.get_turn_order() == CScenarioReader::TurnOrder_First ? com_sck_to_first : com_sck_to_second);
			switch(act_type){
				case CScenarioReader::ActionType_PrintRecvMessage:
					printf("Receive Print\n");
					json_reader.recv_JSON_and_print(next_com_sck);
					break;
				case CScenarioReader::ActionType_WriteRecvMessage:
					printf("Receive Write\n");
					json_reader.recv_JSON_and_write_file(next_com_sck, scenario_reader.get_filename_to_write_recv_msg());
					break;
				case CScenarioReader::ActionType_SendMessage:
					printf("message: %s\n", scenario_reader.get_message_to_send().c_str());
					json_sender.send_JSON(next_com_sck, scenario_reader.get_message_to_send());
					break;
				case CScenarioReader::ActionType_SendFileContents:
					printf("filename: %s\n", scenario_reader.get_filename_to_send().c_str());
					json_sender.send_JSON_from_file(next_com_sck, scenario_reader.get_filename_to_send());
					break;
				case CScenarioReader::ActionType_None:
					printf("Empty Line\n");
					break;
				default:
					printf("Invalid action\n");
					break;
			}
		}
	}
	catch(std::exception& e){
		printf("exception: %s\n", e.what());
	}
	return EXIT_SUCCESS;
}
