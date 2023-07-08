/**
*	@file		local_setting_file.h
*	@brief		heis ローカルモード設定ファイルクラス
*	@author		Ryo Konno
*	@details	ローカルモードの設定ファイルを読み書きするクラス。
*/

#pragma once

#include "setting_file_base.h"
#include "common_structure.h"
#include "setting.h"

/**
*	@brief	ローカルモード設定ファイルクラス
*/
class CLocalSettingFile : public CSettingFileBase<LocalSetting>
{
	// メンバ関数
public:
	// 設定値をローカルモード設定ファイルから読み込む
	LocalSetting load(const std::string& path) const;
	// 設定値をローカルモード設定ファイルに書き込む
	void save(const std::string& path, const LocalSetting& setting) const;

private:
	// 各兵士の初期位置の情報を、盤面上の位置を表す構造体に変換する
	std::vector<BoardPosition> build_init_positions(
		const std::vector<std::string>& init_x_from_csv,
		const std::vector<std::string>& init_y_from_csv
	) const;
};