/**
*	@file		audience_setting_file.cpp
*	@brief		heis 観戦モード設定ファイルクラス
*	@author		Ryo Konno
*	@details	観戦モード設定ファイルを読み書きするクラス。
*/

#include "audience_setting_file.h"
#include "setting_keys.h"

/**
*	@brief 設定値を観戦モード設定ファイルから読み込む
*	@param[in] path 観戦モード設定ファイルのパス
*	@return OnlineSetting 読み込まれた設定値
*/
AudienceSetting CAudienceSettingFile::load(const std::string& path) const
{
	// CSVの読み込み
	CsvRecords records = parse_csv(path);

	// 設定値を設定値構造体に格納する
	AudienceSetting setting;

	setting.server_ip_addr = records[AUDIENCE_SETTING_KEY_SVR_ADDR][0];
	setting.server_port_num = atoi(records[AUDIENCE_SETTING_KEY_SVR_PORT][0].c_str());

	return setting;
}

/**
*	@brief 設定値を観戦モード設定ファイルに書き込む
*	@param[in] path 観戦モード設定ファイルのパス
*	@param[in] setting 書き込む設定値
*/
void CAudienceSettingFile::save(const std::string& path, const AudienceSetting& setting) const
{
	// 現時点で観戦モード設定ファイルに設定値を保存する必要がないため、Do Nothing
	// 保存する必要が生じたら、適宜実装する
}