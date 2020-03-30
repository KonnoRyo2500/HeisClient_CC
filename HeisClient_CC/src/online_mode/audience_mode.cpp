/**
*	@file		audience_mode.cpp
*	@brief		heis 観戦モードクラス
*	@author		Ryo Konno
*	@details	heisの対戦を観戦する．
*/

#include "audience_mode.h"
#include "setting_keys.h"
#include "csv_setting_file_reader.h"
#include "path_generator.h"

/**
*	@def AUDIENCE_SETTING_FILE_NAME
*	@brief 観戦モード設定ファイルの名前
*/
#define AUDIENCE_SETTING_FILE_NAME "audience_setting.csv"

/* public関数 */
/**
*	@brief 観戦メイン処理
*/
void CAudienceMode::play_game()
{
	g_system_log.write_log(CLog::LogType_Infomation, false, "観戦を開始しました");

	// 観戦開始
	initialize_watch();

	// サーバから受信した文字列を表示("finished"は対戦終了を表す文字列)
	std::string msg = "";
	do {
		msg = m_sck->sck_recv();
		printf("%s\n", msg.c_str());
	} while (msg != "finished");

	// 観戦終了
	finalize_watch();

	g_system_log.write_log(CLog::LogType_Infomation, false, "観戦が終了しました");
}

/* private関数 */

/**
*	@brief 観戦の初期化を行う関数
*/
void CAudienceMode::initialize_watch()
{
	CCsvSettingFileReader sr(CPathGenerator::get_exe_path() + AUDIENCE_SETTING_FILE_NAME);
	std::string svr_addr;
	uint16_t svr_port;

	// インスタンスを生成
	m_sck = new CClientSocket();

	// 接続するサーバの情報を設定ファイルから取得
	svr_addr = sr.get_single_value<std::string>(AUDIENCE_SETTING_KEY_SVR_ADDR, 0);
	svr_port = sr.get_single_value<uint16_t>(AUDIENCE_SETTING_KEY_SVR_PORT, 0);

	// サーバに接続
	m_sck->sck_connect(svr_addr, svr_port);
}

/**
*	@brief 観戦の終了処理を行う関数
*/
void CAudienceMode::finalize_watch()
{
	// インスタンスを破棄
	delete m_sck;
	m_sck = NULL;
}