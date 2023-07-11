/**
*	@file		audience_setting_file.h
*	@brief		heis 観戦モード設定ファイルクラス
*	@author		Ryo Konno
*	@details	観戦モード設定ファイルを読み書きするクラス。
*/

#pragma once

#include "setting_file_base.h"
#include "common_structure.h"
#include "setting.h"

/**
*	@brief	観戦モード設定ファイルクラス
*/
class CAudienceSettingFile : public CSettingFileBase<AudienceSetting>
{
	// メンバ関数
public:
	// 設定値を観戦モード設定ファイルから読み込む
	AudienceSetting load(const std::string& path) const;
	// 設定値を観戦モード設定ファイルに書き込む
	void save(const std::string& path, const AudienceSetting& setting) const;
};