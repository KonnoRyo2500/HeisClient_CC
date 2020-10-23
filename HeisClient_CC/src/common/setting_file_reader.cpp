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
*	@param[in] file_name 設定ファイル名
*/
CSettingFileReader::CSettingFileReader(const std::string& file_name)
	:m_key_value()
{
	load_all_value(file_name);
}

/**
*	@brief デストラクタ
*/
CSettingFileReader::~CSettingFileReader()
{
	// 処理なし
}

/* private関数 */
/**
*	@brief すべての値を，設定ファイルから取得する関数
*	@param[in] file_name 設定ファイル名
*/
void CSettingFileReader::load_all_value(const std::string& file_name)
{
	std::ifstream csv_file(file_name);
	if (csv_file.fail()) {
		throw std::runtime_error(cc_common::format("設定ファイルのオープンに失敗しました(ファイル名: %s)", file_name.c_str()));
	}

	std::string str;
	while (std::getline(csv_file, str)) {
		std::vector<std::string> key_value = cc_common::split_string(str, ",");
		// 値は最低1個以上ある必要があるので，キーと要素が両方揃っているためには要素数が2個以上ある必要がある
		if (key_value.size() < 2) {
			throw std::runtime_error(cc_common::format("キーか値の少なくとも一方が欠損しています(キー名: %s)",
				key_value.size() >= 1 ? key_value[0].c_str() : "欠損"));
		}
		remove_space_around_comma(key_value);

		std::string key = key_value[0];
		std::vector<std::string> value;
		std::copy(key_value.begin() + 1, key_value.end(), std::back_inserter(value));
		m_key_value.emplace(key, value);
	}
}

/**
*	@brief 指定されたキーを持つ値を探索する関数
*	@param[in] key キー名
*	@return std::vector<std::string> 値を表すトークンの集合
*	@throws std::runtime_error 指定したキーがファイルになかったとき
*/
std::vector<std::string> CSettingFileReader::search_value(const std::string& key) const
{
	auto it = m_key_value.find(key);
	if (it == m_key_value.end()) {
		throw std::runtime_error(cc_common::format("キーが見つかりませんでした(キー名: %s)", key.c_str()));
	}
	return it->second;
}

/**
*	@brief カンマの前後にある空白類文字を削除する関数
*	@param[out] key_value キーと値全体(カンマによってトークンに分割済み)
*/
void CSettingFileReader::remove_space_around_comma(std::vector<std::string>& key_value) const
{
	for (auto& token : key_value) {
		// 単語の先頭と末尾についている空白を削除する
		token = std::regex_replace(token, std::regex("(^\\s+)|(\\s+$)"), "");
	}
}