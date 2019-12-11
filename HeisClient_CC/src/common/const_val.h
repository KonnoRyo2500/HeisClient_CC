// heis 各種定数
// Author: Ryo Konno
#pragma once

#include<string>

// プログラム中で共通して使う定数をここに記述する

/* 全モード共通で使用する定数 */

// 方向
enum Direction {
	Direction_Up,					// 上
	Direction_Down,					// 下
	Direction_Left,					// 左
	Direction_Right,				// 右
};

/* ローカルモードで使用する定数 */

// フィールドのサイズ(ローカルモードで用いる)
enum LocalFieldSize {
	LocalFieldSize_Width = 20,			// 幅
	LocalFieldSize_Height = 20,			// 高さ
};

// チーム名
// 注意: 最初の2文字が被らないような名前にしておくこと(被るとIDが衝突し，動作が未定義になる)
// TODO: チーム名を設定ファイルで決められるようにする
const std::string LOCAL_MY_TEAM_NAME = "test";			// 自軍
const std::string LOCAL_ENEMY_TEAM_NAME = "enemy";		// 敵軍

/* オンラインモードで使用する定数 */