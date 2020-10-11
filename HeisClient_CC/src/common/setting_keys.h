﻿/**
*	@file		setting_keys.h
*	@brief		heis 設定ファイルキー一覧
*	@author		Ryo Konno
*	@details	設定ファイルの各キーを表す定数マクロを定義する．
*/
#pragma once

/* 観戦モード用 */
/**
*	@def AUDIENCE_SETTING_KEY_SVR_ADDR
*	@brief サーバのIPアドレス
*/
#define AUDIENCE_SETTING_KEY_SVR_ADDR "server_ip_addr"
/**
*	@def AUDIENCE_SETTING_KEY_SVR_PORT
*	@brief サーバのポート番号
*/
#define AUDIENCE_SETTING_KEY_SVR_PORT "server_port_num"

/* オンラインモード用 */
/**
*	@def ONLINE_SETTING_KEY_SVR_ADDR
*	@brief サーバのIPアドレス
*/
#define ONLINE_SETTING_KEY_SVR_ADDR "server_ip_addr"
/**
*	@def ONLINE_SETTING_KEY_SVR_PORT
*	@brief サーバのポート番号
*/
#define ONLINE_SETTING_KEY_SVR_PORT "server_port_num"
/**
*	@def ONLINE_SETTING_KEY_TEAM_NAME
*	@brief 名前要求で，サーバに伝えるチーム名
*/
#define ONLINE_SETTING_KEY_TEAM_NAME "team_name"

/* ローカルモード用 */
/**
*	@def LOCAL_SETTING_KEY_BOARD_WIDTH
*	@brief 盤面の幅
*/
#define LOCAL_SETTING_KEY_BOARD_WIDTH "board_width"
/**
*	@def LOCAL_SETTING_KEY_BOARD_HEIGHT
*	@brief 盤面の高さ
*/
#define LOCAL_SETTING_KEY_BOARD_HEIGHT "board_height"
/**
*	@def LOCAL_SETTING_KEY_MY_NAME
*	@brief 自チームの名前
*/
#define LOCAL_SETTING_KEY_MY_NAME "my_team_name"
/**
*	@def LOCAL_SETTING_KEY_ENEMY_NAME
*	@brief 敵チームの名前
*/
#define LOCAL_SETTING_KEY_ENEMY_NAME "enemy_team_name"
/**
*	@def LOCAL_SETTING_KEY_MY_INIT_POS
*	@brief 自チームの兵士初期位置
*/
#define LOCAL_SETTING_KEY_MY_INIT_POS "my_infantries_init_position"
/**
*	@def LOCAL_SETTING_KEY_ENEMY_INIT_POS
*	@brief 敵チームの兵士初期位置
*/
#define LOCAL_SETTING_KEY_ENEMY_INIT_POS "enemy_infantries_init_position"
/**
*	@def LOCAL_SETTING_KEY_FIRST_TEAM
*	@brief 先攻となるチーム名
*/
#define LOCAL_SETTING_KEY_FIRST_TEAM "first_turn_team"
