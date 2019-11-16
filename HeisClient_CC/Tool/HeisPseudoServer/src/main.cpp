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
		CScenarioReader sr("scenario_sample.txt");
		CScenarioReader::ActionType act_type;
		CJsonSender js;
		CJsonReceiver jr;
		
		// ソケットの初期化
		// クライアントは1人しか接続してこない想定
		// TODO: ポート番号を設定ファイルから取得できるようにする
		g_sck.sck_bind(20000, IP_SELF);
		g_sck.sck_listen();
		g_sck.sck_accept();
		
		while((act_type = sr.get_next_aciton_type()) != CScenarioReader::ActionType_AllActionDone){
			printf("type: %d\n", act_type);
			switch(act_type){
				case CScenarioReader::ActionType_PrintRecvMessage:
					printf("Receive Print\n");
					jr.recv_JSON_and_print();
					break;
				case CScenarioReader::ActionType_WriteRecvMessage:
					printf("Receive Write\n");
					jr.recv_JSON_and_write_file(sr.get_filename_to_write_recv_msg());
					break;
				case CScenarioReader::ActionType_SendMessage:
					printf("message: %s\n", sr.get_message_to_send().c_str());
					js.send_JSON(sr.get_message_to_send());
					break;
				case CScenarioReader::ActionType_SendFileContents:
					printf("filename: %s\n", sr.get_filename_to_send().c_str());
					js.send_JSON_from_file(sr.get_filename_to_send());
					break;
				case CScenarioReader::ActionType_None:
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