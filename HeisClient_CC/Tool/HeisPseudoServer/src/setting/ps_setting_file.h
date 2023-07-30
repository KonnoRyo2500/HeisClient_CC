/**
*	@file		ps_setting_file.h
*	@brief		heis PS設定ファイルクラス
*	@author		Ryo Konno
*	@details	PSの設定ファイルを読み書きするクラス。
*/

#pragma once

#include "setting_file_base.h"

/**
*	@struct PsSetting
*	@brief PS設定値格納用構造体
*/
struct PsSetting {
	//! シナリオファイル名
	std::string scenario_file_name;
	//! 先攻からの接続を待ち受けるポート番号
	uint16_t listen_port_num_first;
	//! 後攻からの接続を待ち受けるポート番号
	uint16_t listen_port_num_second;
	//! 先攻からの接続を待ち受けるIPアドレス
	std::string listen_addr_first;
	//! 後攻からの接続を待ち受けるIPアドレス
	std::string listen_addr_second;
};

/**
*	@brief	PS設定ファイルクラス
*/
class CPsSettingFile : public CSettingFileBase<PsSetting>
{
	// メンバ関数
public:
	// 設定値をPS設定ファイルから読み込む
	PsSetting load(const std::string& path) const;
	// 設定値をPS設定ファイルに書き込む
	void save(const std::string& path, const PsSetting& setting) const;
};