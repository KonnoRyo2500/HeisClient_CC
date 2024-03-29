/**
*	@file		audience_mode.cpp
*	@brief		heis 観戦モードクラス
*	@author		Ryo Konno
*	@details	heisの対戦を観戦する．
*/

#include "audience_mode.h"
#include "setting_keys.h"
#include "audience_setting_file.h"
#include "path.h"

/**
*	@def AUDIENCE_SETTING_FILE_NAME
*	@brief 観戦モード設定ファイルの名前
*/
#define AUDIENCE_SETTING_FILE_NAME "audience_setting.csv"

/**
*	@brief 観戦メイン処理
*/
void CAudienceMode::play_game()
{
	CLog::write(CLog::LogLevel_Information, "観戦を開始しました");

	// 設定ファイルの読み込み
	AudienceSetting setting = CAudienceSettingFile().load(
		join({CC_SETTING_DIR, AUDIENCE_SETTING_FILE_NAME})
	);

	// 観戦開始
	CSocket socket;
	socket.connect(setting.server_ip_addr, setting.server_port_num);

	// サーバから受信した文字列を表示("finished"は対戦終了を表す文字列)
	std::string msg = "";
	do {
		msg = socket.recv('\n');
		printf("%s\n", msg.c_str());
	} while (msg != "finished");

	CLog::write(CLog::LogLevel_Information, "観戦が終了しました");
}