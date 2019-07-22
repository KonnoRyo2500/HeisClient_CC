// heis 各種定数
// Author: Ryo Konno
#pragma once

// プログラム中で共通して使う定数をここに記述する

/* 全モード共通で使用する定数 */

//
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

/* オンラインモードで使用する定数 */