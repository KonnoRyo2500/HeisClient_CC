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
		CScenarioReader::ActionType act_type;
		while((act_type = sr.get_next_aciton_type()) != CScenarioReader::ActionType_AllActionDone){
			printf("type: %d\n", act_type);
			switch(act_type){
				case CScenarioReader::ActionType_PrintRecvMessage:
					printf("Receive Print\n");
					break;
				case CScenarioReader::ActionType_WriteRecvMessage:
					printf("Receive Write\n");
					break;
				case CScenarioReader::ActionType_SendMessage:
					printf("message: %s\n", sr.get_message_to_send().c_str());
					break;
				case CScenarioReader::ActionType_SendFileContents:
					printf("filename: %s\n", sr.get_filename_to_send().c_str());
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