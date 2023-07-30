/**
*	@file		ps_setting_file.cpp
*	@brief		heis PS設定ファイルクラス
*	@author		Ryo Konno
*	@details	PSの設定ファイルを読み書きするクラス。
*/

#include "ps_setting_file.h"
#include "ps_setting_keys.h"

/**
*	@brief 設定値をPS設定ファイルから読み込む
*	@param[in] path PS設定ファイルのパス
*	@return PsSetting 読み込まれた設定値
*/
PsSetting CPsSettingFile::load(const std::string& path) const
{
	// CSVの読み込み
	CsvRecords records = parse_csv(path);

	// 設定値を設定値構造体に格納する
	PsSetting setting;

	setting.scenario_file_name = records[PS_SETTING_KEY_SCENARIO_NAME][0];
	setting.listen_port_num_first = atoi(records[PS_SETTING_KEY_LISTEN_PORT_FIRST][0].c_str());
	setting.listen_port_num_second = atoi(records[PS_SETTING_KEY_LISTEN_PORT_SECOND][0].c_str());
	setting.listen_addr_first = records[PS_SETTING_KEY_LISTEN_ADDR_FIRST][0];
	setting.listen_addr_second = records[PS_SETTING_KEY_LISTEN_ADDR_SECOND][0];

	return setting;
}

/**
*	@brief 設定値をPS設定ファイルに書き込む
*	@param[in] path PS設定ファイルのパス
*	@param[in] setting 書き込む設定値
*/
void CPsSettingFile::save(const std::string& path, const PsSetting& setting) const
{
	// 現時点でPS設定ファイルに設定値を保存する必要がないため、Do Nothing
	// 保存する必要が生じたら、適宜実装する
}