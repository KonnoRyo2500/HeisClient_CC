/**
*	@file		setting_file_reader.cpp
*	@brief		設定ファイル読み出しクラス
*	@author		Ryo Konno
*	@details	設定ファイルから設定値を読み出す．
*/
#include "setting_file_reader.h"

#include <fstream>
#include <algorithm>
#include <regex>

/* public関数 */
/**
*	@brief コンストラクタ
*	@param[in] path 設定ファイルのパス
*/
CSettingFileReader::CSettingFileReader(const std::string& path)
	: m_key_value()
	, m_path(path)
{
	load(path);
}

/**
*	@brief デストラクタ
*/
CSettingFileReader::~CSettingFileReader()
{
	// 処理なし
}

/**
*	@brief 指定されたキーが存在するか判定する関数
*	@param[in] key キー名
*	@return bool キーが存在するか(存在する: true, 存在しない: false)
*/
bool CSettingFileReader::exists_key(const std::string& key) const
{
	return m_key_value.find(key) != m_key_value.end();
}

/* private関数 */
/**
*	@brief 設定ファイルからキーと値をロードする関数
*	@param[in] path 設定ファイルのパス
*/
void CSettingFileReader::load(const std::string& path)
{
	std::ifstream setting_file(path);

	if (setting_file.fail()) {
		throw std::runtime_error(cc_common::format("設定ファイル%sのオープンに失敗しました", path.c_str()));
	}

	std::string line;
	while (std::getline(setting_file, line)) {
		std::smatch smatch;

		if (std::regex_match(line, smatch, std::regex("^\\s*(\\w+)\\s+([^\\s]+)\\s*$"))) {
			// smatch[0] -> 文字列全体, smatch[1] -> キー, smatch[2] -> 値
			std::string key = smatch[1].str();
			std::string value = smatch[2].str();

			m_key_value[key] = value;
		}
	}
}