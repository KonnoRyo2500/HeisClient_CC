/**
*	@file		local_setting_file.cpp
*	@brief		heis ローカルモード設定ファイルクラス
*	@author		Ryo Konno
*	@details	ローカルモードの設定ファイルを読み書きするクラス。
*/

#include "local_setting_file.h"
#include "setting_keys.h"

/**
*	@brief 設定値をローカルモード設定ファイルから読み込む
*	@param[in] path ローカルモード設定ファイルのパス
*	@return LocalSetting 読み込まれた設定値
*/
LocalSetting CLocalSettingFile::load(const std::string& path) const
{
	// CSVの読み込み
	CsvRecords records = parse_csv(path);

	// 設定値を設定値構造体に格納する
	LocalSetting setting;

	setting.my_team_name = records[LOCAL_SETTING_KEY_MY_NAME][0];
	setting.enemy_team_name = records[LOCAL_SETTING_KEY_ENEMY_NAME][0];
	setting.my_team_ai_impl = records[LOCAL_SETTING_KEY_MY_AI_IMPL][0];
	setting.enemy_team_ai_impl = records[LOCAL_SETTING_KEY_ENEMY_AI_IMPL][0];
	setting.board_width = atoi(records[LOCAL_SETTING_KEY_BOARD_WIDTH][0].c_str());
	setting.board_height = atoi(records[LOCAL_SETTING_KEY_BOARD_HEIGHT][0].c_str());
	setting.my_team_init_pos = build_init_positions(
		records[LOCAL_SETTING_KEY_MY_INIT_COORD_X],
		records[LOCAL_SETTING_KEY_MY_INIT_COORD_Y]
	);
	setting.enemy_team_init_pos = build_init_positions(
		records[LOCAL_SETTING_KEY_ENEMY_INIT_COORD_X],
		records[LOCAL_SETTING_KEY_ENEMY_INIT_COORD_Y]
	);
	setting.first_turn_team = records[LOCAL_SETTING_KEY_FIRST_TEAM][0];

	return setting;
}

/**
*	@brief 設定値をローカルモード設定ファイルに書き込む
*	@param[in] path ローカルモード設定ファイルのパス
*	@param[in] setting 書き込む設定値
*/
void CLocalSettingFile::save(const std::string& path, const LocalSetting& setting) const
{
	// 現時点でローカルモード設定ファイルに設定値を保存する必要がないため、Do Nothing
	// 保存する必要が生じたら、適宜実装する
}

/**
*	@brief 各兵士の初期位置の情報を、盤面上の位置を表す構造体に変換する
*	@param[in] init_x_from_csv 各兵士のx座標
*	@param[in] init_y_from_csv 各兵士のy座標
*	@return std::vector<Coordinate2D> 各兵士の初期位置
*/
std::vector<Coordinate2D> CLocalSettingFile::build_init_positions(
	const std::vector<std::string>& init_x_from_csv,
	const std::vector<std::string>& init_y_from_csv
) const
{
	// x座標・y座標の数が同じでなければエラー
	if (init_x_from_csv.size() != init_y_from_csv.size()) {
		throw std::runtime_error("兵士の初期配置座標のx座標・y座標はそれぞれ同じ数だけ設定する必要があります");
	}

	// 指定されたx座標・y座標を盤面上の位置に変換する
	std::vector<Coordinate2D> init_positions;
	for (int i = 0; i < init_x_from_csv.size(); i++) {
		Coordinate2D pos = Coordinate2D(
			atoi(init_x_from_csv[i].c_str()),
			atoi(init_y_from_csv[i].c_str())
		);
		
		init_positions.push_back(pos);
	}

	return init_positions;
}