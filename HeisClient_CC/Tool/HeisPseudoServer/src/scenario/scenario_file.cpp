/**
*	@file		scenario_file.h
*	@brief		PS シナリオファイルクラス
*	@author		Ryo Konno
*	@details	PSのシナリオファイルに関する各種操作を提供する。
*/

#include "scenario_file.h"
#include "string_utils.h"

#include <fstream>
#include <stdexcept>

/**
*	@brief シナリオファイルをロードする
*	@param[in] path シナリオファイルのパス
*	@return std::vector<std::vector<std::string>> シナリオファイルから読み込んだコマンド
*/
std::vector<std::vector<std::string>> CScenarioFile::load(const std::string& path)
{
	// シナリオファイルを開く
	std::ifstream ifs(path);
	if (ifs.fail()) {
		throw std::runtime_error("シナリオファイルのオープンに失敗しました");
	}

	// シナリオファイルからコマンドを読み込んでパースする
	std::string scenario_line;
	std::vector<std::vector<std::string>> commands;
	while(std::getline(ifs, scenario_line)) {
		// UNIX環境, かつ設定ファイルがCRLFの時の対策
		// 上記の場合、そのままだとcsv_lineの末尾にCRが付くため、これを削除する
		if (scenario_line[scenario_line.size() - 1] == '\r') {
			scenario_line.erase(scenario_line.size() - 1);
		}

		// ここでは文字列のパースのみ行う
		// 各コマンドの引数やフォーマットのチェックは各コマンドの実行クラスで行う
		std::vector<std::string> command = CStringUtil::split(scenario_line, ' ');
		if (command.size() == 0) {
			// 空行
			continue;
		}

		commands.push_back(command);
	}

	return commands;
}
