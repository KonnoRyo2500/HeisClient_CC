/**
*	@file		path_generator.cpp
*	@brief		heis パス生成クラス
*	@author		Ryo Konno
*	@details	特定のファイルへのパスを生成する．
*/
#include "path_generator.h"
#include "heis_client_exception.h"

#include <climits>
#ifdef WIN32
#include <Windows.h>
#else
#include <unistd.h>
#endif // WIN32

/* public関数 */

/**
*	@brief 実行ファイルのパスを返す関数
*	@return std::string 実行ファイルのパス 
*/
std::string CPathGenerator::get_exe_path()
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
		throw CHeisClientException("シナリオファイルのパス取得に失敗しました");
	}
#endif
}

/* private関数 */