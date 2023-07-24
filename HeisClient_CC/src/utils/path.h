/**
*	@file		path.h
*	@brief		heis パス操作
*	@author		Ryo Konno
*	@details	パス操作のための各種関数を定義する。
*/

#pragma once

#include <string>
#include <vector>

// パス文字列を結合する
std::string join(const std::vector<std::string>& paths);
// 実行ファイルのディレクトリを得る
std::string build_exe_dir();

/* ディレクトリ名 */
#ifdef WIN32
/**
*	@def CC_ROOT_DIR
*	@brief CCのルートディレクトリ
*/
#define CC_ROOT_DIR join({build_exe_dir(), "..", "..", "..", "..", ".."})

/**
*	@def CC_PROJECT_DIR
*	@brief CCのプロジェクトディレクトリ
*/
#define CC_PROJECT_DIR join({CC_ROOT_DIR, "project_win"})
#else
/**
*	@def CC_ROOT_DIR
*	@brief CCのルートディレクトリ
*/
#define CC_ROOT_DIR join({build_exe_dir(), "..", "..", ".."})

/**
*	@def CC_PROJECT_DIR
*	@brief CCのプロジェクトディレクトリ
*/
#define CC_PROJECT_DIR join({CC_ROOT_DIR, "project_linux"})
#endif // WIN32

/**
*	@def CC_SETTING_DIR
*	@brief 設定ファイルのディレクトリ名
*/
#define CC_SETTING_DIR join({CC_ROOT_DIR, "setting", "CC"})

