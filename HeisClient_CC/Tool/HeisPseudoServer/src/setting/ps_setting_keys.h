/**
*	@file		ps_setting_keys.h
*	@brief		heis 設定ファイルキー一覧
*	@author		Ryo Konno
*	@details	設定ファイルの各キーを表す定数マクロを定義する。
*/

#pragma once

/**
*	@def PS_SETTING_KEY_SCENARIO_NAME
*	@brief シナリオファイル名
*/
#define PS_SETTING_KEY_SCENARIO_NAME "scenario_file_name"

/**
*	@def PS_SETTING_KEY_LISTEN_PORT_FIRST
*	@brief 先攻からの接続を待ち受けるポート番号
*/
#define PS_SETTING_KEY_LISTEN_PORT_FIRST "listen_port_num_first"

/**
*	@def PS_SETTING_KEY_LISTEN_PORT_SECOND
*	@brief 後攻からの接続を待ち受けるポート番号
*/
#define PS_SETTING_KEY_LISTEN_PORT_SECOND "listen_port_num_second"

/**
*	@def PS_SETTING_KEY_LISTEN_ADDR_FIRST
*	@brief 先攻からの接続を待ち受けるIPアドレス
*/
#define PS_SETTING_KEY_LISTEN_ADDR_FIRST "listen_addr_first"

/**
*	@def PS_SETTING_KEY_LISTEN_ADDR_SECOND
*	@brief 後攻からの接続を待ち受けるIPアドレス
*/
#define PS_SETTING_KEY_LISTEN_ADDR_SECOND "listen_addr_second"
