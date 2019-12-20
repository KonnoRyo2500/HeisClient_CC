// heis 設定ファイルキー一覧
// Author: Ryo Konno
#pragma once

/* オンラインモード用 */
// サーバのIPアドレス
#define ONLINE_SETTING_KEY_SVR_ADDR "server_ip_addr"
// サーバのポート番号
#define ONLINE_SETTING_KEY_SVR_PORT "server_port_num"
// 名前要求で，サーバに伝えるチーム名
#define ONLINE_SETTING_KEY_TEAM_NAME "team_name"

/* ローカルモード用 */
// フィールドの幅
#define LOCAL_SETTING_KEY_FIELD_WIDTH "field_width"
// フィールドの高さ
#define LOCAL_SETTING_KEY_FIELD_HEIGHT "field_height"
// 自チームの名前
#define LOCAL_SETTING_KEY_MY_NAME "my_team_name"
// 敵チームの名前
#define LOCAL_SETTING_KEY_ENEMY_NAME "enemy_team_name"
// 自チームの兵士初期位置
#define LOCAL_SETTING_KEY_MY_INIT_POS "my_infantries_init_position"
// 敵チームの兵士初期位置
#define LOCAL_SETTING_KEY_ENEMY_INIT_POS "enemy_infantries_init_position"
// 先攻となるチーム名
#define LOCAL_SETTING_KEY_FIRST_TEAM "first_turn_team"