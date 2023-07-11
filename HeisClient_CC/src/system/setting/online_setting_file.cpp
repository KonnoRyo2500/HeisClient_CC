/**
*	@file		online_setting_file.cpp
*	@brief		heis オンラインモード設定ファイルクラス
*	@author		Ryo Konno
*	@details	オンラインモードの設定ファイルを読み書きするクラス。
*/

#include "online_setting_file.h"
#include "setting_keys.h"

/**
*	@brief 設定値をオンラインモード設定ファイルから読み込む
*	@param[in] path オンラインモード設定ファイルのパス
*	@return OnlineSetting 読み込まれた設定値
*/
OnlineSetting COnlineSettingFile::load(const std::string& path) const
{
	// CSVの読み込み
	CsvRecords records = parse_csv(path);

	// 設定値を設定値構造体に格納する
	OnlineSetting setting;

	setting.server_ip_addr = records[ONLINE_SETTING_KEY_SVR_ADDR][0];
	setting.server_port_num = atoi(records[ONLINE_SETTING_KEY_SVR_PORT][0].c_str());
	setting.team_name = records[ONLINE_SETTING_KEY_TEAM_NAME][0];
	setting.ai_impl = records[ONLINE_SETTING_KEY_AI_IMPL][0];

	return setting;
}

/**
*	@brief 設定値をオンラインモード設定ファイルに書き込む
*	@param[in] path オンラインモード設定ファイルのパス
*	@param[in] setting 書き込む設定値
*/
void COnlineSettingFile::save(const std::string& path, const OnlineSetting& setting) const
{
	// 現時点でオンラインモード設定ファイルに設定値を保存する必要がないため、Do Nothing
	// 保存する必要が生じたら、適宜実装する
}