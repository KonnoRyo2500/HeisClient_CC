/**
*	@file		main.cpp
*	@brief		PS メイン処理
*	@author		Ryo Konno
*	@details	PSのメイン処理を定義する。
*/

#include "path.h"
#include "ps_setting_file.h"
#include "socket.h"
#include "scenario_file.h"
#include "command_executor_factory.h"

#include <stdexcept>

/**
*	@def PS_SETTING_FILE_NAME
*	@brief PS設定ファイルの名前
*/
#define PS_SETTING_FILE_NAME "ps_setting.csv"

/**
*	@brief メイン処理
*/
int main()
{
	try{
		// 設定ファイルを読み込む
		PsSetting setting = CPsSettingFile().load(
			join({PS_SETTING_DIR, PS_SETTING_FILE_NAME})
		);

		// 先攻クライアントからの接続を受け付ける
		CSocket socket_first, socket_second;
		printf(
			"CCをIPアドレス: %s, ポート番号: %dに接続してください\n",
			setting.listen_addr_first.c_str(),
			setting.listen_port_num_first
		);
		socket_first.bind(
			setting.listen_port_num_first,
			setting.listen_addr_first
		);
		socket_first.listen();
		socket_first.accept();

		// 後攻クライアントからの接続を受け付ける
		printf(
			"CCをIPアドレス: %s, ポート番号: %dに接続してください\n",
			setting.listen_addr_second.c_str(),
			setting.listen_port_num_second
		);
		socket_second.bind(
			setting.listen_port_num_second,
			setting.listen_addr_second
		);
		socket_second.listen();
		socket_second.accept();

		// シナリオファイルからコマンドを読み込む
		printf("コマンド実行開始\n");
		std::vector<std::vector<std::string>> commands = CScenarioFile().load(
			join({PS_SCENARIO_DIR, setting.scenario_file_name})
		);

		// 各コマンドの実行
		CCommandExecutorFactory factory = CCommandExecutorFactory();
		for (auto& cmd : commands) {
			CCommandExecutorBase* executor = factory.create_instance(
				cmd[0],
				socket_first,
				socket_second
			);

			if (executor == NULL) {
				fprintf(stderr, "コマンド %s は未定義です\n", cmd[0].c_str());
				continue;
			}

			executor->execute(cmd);

			delete executor;
			executor = NULL;
		}
		printf("コマンドの実行が完了しました\n");
	}
	catch(std::exception& e){
		fprintf(stderr, "PSの実行中にエラーが発生しました(内容: %s)\n", e.what());
		return EXIT_FAILURE;
	}

	return EXIT_SUCCESS;
}
