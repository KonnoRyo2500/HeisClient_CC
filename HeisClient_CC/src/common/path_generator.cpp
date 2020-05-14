﻿/**
*	@file		path_generator.cpp
*	@brief		heis パス生成処理
*	@author		Ryo Konno
*	@details	特定のファイルへのパスを生成する．
*/
#include "path_generator.h"

#include <climits>
#include <stdexcept>
#ifdef WIN32
#include <Windows.h>
#else
#include <unistd.h>
#endif // WIN32

/* static関数 */
//! 実行ファイルのディレクトリを取得
static std::string get_exe_dir();

/**
*	@brief プロジェクトのディレクトリを返す関数
*	@return std::string プロジェクトのディレクトリ
*/
std::string get_project_dir()
{
#ifdef WIN32
	return get_exe_dir() + "..\\..\\..\\";
#else
	return get_exe_dir() + "../";
#endif
}

/**
*	@brief 実行ファイルのディレクトリを返す関数
*	@return std::string 実行ファイルのディレクトリ
*/
static std::string get_exe_dir()
{
#ifdef WIN32
	char path_buf[MAX_PATH] = { 0 };

	GetModuleFileName(NULL, path_buf, sizeof(path_buf));
	std::string path = std::string(path_buf).substr(0, std::string(path_buf).find_last_of("\\"));
	return path + "\\";
#else
	char path_buf[PATH_MAX] = { 0 };

	if (readlink("/proc/self/exe", path_buf, sizeof(path_buf) - 1) > 0) {
		std::string path = std::string(path_buf).substr(0, std::string(path_buf).find_last_of("/"));
		return path + "/";
}
	else {
		throw std::runtime_error("シナリオファイルのパス取得に失敗しました");
	}
#endif
}