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
std::string join(std::vector<std::string> paths);
// CCのルートディレクトリを得る
std::string build_cc_root_dir();

/* ディレクトリ名 */
/**
*	@def 設定ファイルのディレクトリ名
*/
#define CC_SETTING_DIR join({build_cc_root_dir(), "setting", "CC"})

#ifdef WIN32
/**
*	@def CCのプロジェクトディレクトリ
*/
#define CC_PROJECT_DIR join({build_cc_root_dir(), "project_win"})
#else
/**
*	@def CCのプロジェクトディレクトリ
*/
#define CC_PROJECT_DIR join({build_cc_root_dir(), "project_linux"})
#endif // WIN32