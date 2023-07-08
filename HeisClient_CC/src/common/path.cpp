/**
*	@file		path.cpp
*	@brief		heis パス操作
*	@author		Ryo Konno
*	@details	パス操作のための各種関数を定義する。
*/

#include "path.h"

#include <climits>
#ifdef WIN32
#include <Windows.h>
#else
#include <unistd.h>
#endif // WIN32
#include <stdarg.h>
#include <filesystem>

/**
*	@brief パス文字列を結合する
*	@param[in] paths パス文字列
*	@return std::string 結合後のパス文字列
*/
std::string join(const std::vector<std::string>& paths)
{
	std::filesystem::path joined_path;

	for (std::string p : paths) {
		joined_path.append(p);
	}
	joined_path = joined_path.lexically_normal();

	return joined_path.string();
}


/**
*	@brief 実行ファイルのディレクトリを返す関数
*	@return std::string 実行ファイルのディレクトリ
*/
std::string build_cc_root_dir()
{
#ifdef WIN32
	char exe_path_c[MAX_PATH] = { 0 };

	GetModuleFileName(NULL, exe_path_c, sizeof(exe_path_c));
	std::string exe_path_s = std::string(exe_path_c);// <root>\project_win\bin\Debug or Release\x86 or x64\HeisClient_CC.exe
	std::string root_dir = join({exe_path_s, "..", "..", "..", "..", ".."});

	return root_dir;
#else
	char exe_path_c[PATH_MAX] = { 0 };

	if (readlink("/proc/self/exe", exe_path_c, sizeof(exe_path_c) - 1) > 0) {
		std::string exe_path_s = std::string(exe_path_c); // <root>\project_linux\bin\HeisClient_CC
		std::string root_dir = join({exe_path_s, "..", "..", ".."});
		return root_dir;
	}
	else {
		throw std::runtime_error("実行ファイルのパス取得に失敗しました");
	}
#endif
}