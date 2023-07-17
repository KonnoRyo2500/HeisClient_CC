/**
*	@file		setting.h
*	@brief		heis 設定ファイル構造体
*	@author		Ryo Konno
*	@details	各種ゲームモードでの設定値を格納するための構造体を定義する。
*/

#pragma once

#include <string>
#include <vector>

#include "coodinate_2d.h"

/**
*	@struct LocalSetting
*	@brief ローカルモード設定値格納用構造体
*/
struct LocalSetting {
	//! 自チーム名
	std::string my_team_name;
	//! 敵チーム名
	std::string enemy_team_name;
	//! 自チームのAI実装
	std::string my_team_ai_impl;
	//! 敵チームのAI実装
	std::string enemy_team_ai_impl;
	//! 盤面の横幅
	uint16_t board_width;
	//! 盤面の高さ
	uint16_t board_height;
	//! 自チーム兵士の初期位置
	std::vector<Coordinate2D> my_team_init_pos;
	//! 敵チーム兵士の初期位置
	std::vector<Coordinate2D> enemy_team_init_pos;
	//! 先攻となるチーム名
	std::string first_turn_team;
};

/**
*	@struct OnlineSetting
*	@brief オンラインモード設定値格納用構造体
*/
struct OnlineSetting {
	//! サーバのIPアドレス
	std::string server_ip_addr;
	//! サーバのポート番号
	uint16_t server_port_num;
	//! チーム名
	std::string team_name;
	//! AI実装
	std::string ai_impl;
};

/**
*	@struct AudienceSetting
*	@brief 観戦モード設定値格納用構造体
*/
struct AudienceSetting {
	//! サーバのIPアドレス
	std::string server_ip_addr;
	//! サーバのポート番号
	uint16_t server_port_num;
};