﻿/**
*	@file		local_setting.h
*	@brief		heis ローカルモード設定構造体
*	@author		Ryo Konno
*	@details	ローカルモード実行時の設定を保持する構造体を定義する．
*/
#pragma once

#include "common_structure.h"

#include <string>
#include <vector>

/**
*	@struct LocalSetting
*	@brief ローカルモード設定構造体
*/
struct LocalSetting {
	//! 自チーム名
	std::string my_team_name;
	//! 敵チーム名
	std::string enemy_team_name;
	//! フィールドの横幅
	uint16_t field_width;
	//! フィールドの高さ
	uint16_t field_height;
	//! 自チーム兵士の初期位置
	std::vector<FieldPosition> my_team_init_pos;
	//! 敵チーム兵士の初期位置
	std::vector<FieldPosition> enemy_team_init_pos;
	//! 自分のチームが先攻か
	bool is_my_team_first;
};