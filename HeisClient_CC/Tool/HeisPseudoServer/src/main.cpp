// heisクライアント用疑似サーバ メイン処理
// Author: Ryo Konno

#include "scenario_reader.h"
#include "JSON_sender.h"
#include "JSON_receiver.h"
#include "server_socket.h"

#define IP_SELF "127.0.0.1"

/* グローバル変数 */
// CCとの通信用ソケット
CServerSocket g_sck;

/*
	メイン関数
*/
int main(int argc, char **argv)
{
	try{
		CScenarioReader scenario_reader("scenario_sample.txt");
		CScenarioReader::ActionType act_type;
		CJsonSender json_sender;
		CJsonReceiver json_reader;
		
		// ソケットの初期化
		// クライアントは1人しか接続してこない想定
		// TODO: ポート番号を設定ファイルから取得できるようにする
		g_sck.sck_bind(20000, IP_SELF);
		g_sck.sck_listen();
		g_sck.sck_accept();
		
		while((act_type = scenario_reader.get_next_aciton_type()) != CScenarioReader::ActionType_AllActionDone){
			printf("type: %d\n", act_type);
			switch(act_type){
				case CScenarioReader::ActionType_PrintRecvMessage:
					printf("Receive Print\n");
					json_reader.recv_JSON_and_print();
					break;
				case CScenarioReader::ActionType_WriteRecvMessage:
					printf("Receive Write\n");
					json_reader.recv_JSON_and_write_file(scenario_reader.get_filename_to_write_recv_msg());
					break;
				case CScenarioReader::ActionType_SendMessage:
					printf("message: %s\n", scenario_reader.get_message_to_send().c_str());
					json_sender.send_JSON(scenario_reader.get_message_to_send());
					break;
				case CScenarioReader::ActionType_SendFileContents:
					printf("filename: %s\n", scenario_reader.get_filename_to_send().c_str());
					json_sender.send_JSON_from_file(scenario_reader.get_filename_to_send());
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