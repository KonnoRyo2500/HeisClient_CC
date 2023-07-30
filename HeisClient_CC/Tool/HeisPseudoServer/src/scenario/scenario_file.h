/**
*	@file		scenario_file.h
*	@brief		PS シナリオファイルクラス
*	@author		Ryo Konno
*	@details	PSのシナリオファイルに関する各種操作を提供する。
*/

#pragma once

#include <vector>
#include <string>

/**
*	@brief シナリオファイルクラス
*/
class CScenarioFile
{
	// クラス関数
public:
	// シナリオファイルをロードする
	static std::vector<std::vector<std::string>> load(const std::string& path);
};
