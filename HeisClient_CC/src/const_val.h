// heis 各種定数
// Author: Ryo Konno
#pragma once

// プログラム中で共通して使う定数をここに記述する

// 方向
enum Direction {
	Direction_Up,					// 上
	Direction_Down,					// 下
	Direction_Left,					// 左
	Direction_Right,				// 右
};

// フィールドに関する定数(ローカルモードで用いる)
enum FieldParam {
	FieldParam_Width = 20,			// 幅
	FieldParam_Height = 20,			// 高さ
};