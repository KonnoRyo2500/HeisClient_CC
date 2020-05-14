/**
*	@file		path_generator.h
*	@brief		heis パス生成処理
*	@author		Ryo Konno
*	@details	特定のディレクトリへのパスを生成する．
*/
#pragma once

#include <string>

// ログディレクトリの取得
std::string get_log_dir();
// 設定ディレクトリの取得
std::string get_setting_dir();