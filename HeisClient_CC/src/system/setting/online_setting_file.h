/**
*	@file		online_setting_file.h
*	@brief		heis オンラインモード設定ファイルクラス
*	@author		Ryo Konno
*	@details	オンラインモードの設定ファイルを読み書きするクラス。
*/

#pragma once

#include "setting_file_base.h"
#include "setting.h"

/**
*	@brief	オンラインモード設定ファイルクラス
*/
class COnlineSettingFile : public CSettingFileBase<OnlineSetting>
{
	// メンバ関数
public:
	// 設定値をオンラインモード設定ファイルから読み込む
	OnlineSetting load(const std::string& path) const;
	// 設定値をオンラインモード設定ファイルに書き込む
	void save(const std::string& path, const OnlineSetting& setting) const;
};