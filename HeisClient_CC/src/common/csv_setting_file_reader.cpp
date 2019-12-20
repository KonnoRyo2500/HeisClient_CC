// CSV設定ファイルクラス
// Author: Ryo Konno

#include "csv_setting_file_reader.h"

#include <fstream>
#include <algorithm>
#include <regex>

/* public関数 */
/*
	コンストラクタ
	引数1: const std::string& file_name 設定ファイル名
	例外: ファイルオープンに失敗したとき
*/
CCsvSettingFileReader::CCsvSettingFileReader(const std::string& file_name)
	:m_key_value()
{
	load_all_value(file_name);
}

/*
	デストラクタ
	引数なし
*/
CCsvSettingFileReader::~CCsvSettingFileReader()
{
	// 処理なし
}

/* private関数 */

/*
	すべての値を，設定ファイルから取得する関数
	引数1: const std::string& file_name 設定ファイル名
	返り値なし
*/
void CCsvSettingFileReader::load_all_value(const std::string& file_name)
{
	std::ifstream csv_file(file_name);
	if (csv_file.fail()) {
		throw CHeisClientException("設定ファイルのオープンに失敗しました(ファイル名: %s)", file_name.c_str());
	}

	std::string str;
	while (std::getline(csv_file, str)) {
		CTokenManager tm;
		token_array_t key_value = tm.split_string(str, ",");
		// 値は最低1個以上ある必要があるので，キーと要素が両方揃っているためには要素数が2個以上ある必要がある
		if (key_value.size() < 2) {
			throw CHeisClientException("キーか値の少なくとも一方が欠損しています(キー名: %s)", 
				key_value.size() >= 1 ? tm.get_single_token(key_value, 0).c_str() : "欠損");
		}
		remove_space_around_comma(key_value);

		std::string key = tm.get_single_token(key_value, 0);
		token_array_t value;
		std::copy(key_value.begin() + 1, key_value.end(), std::back_inserter(value));
		m_key_value.emplace(key, value);
	}
}

/*
	指定されたキーを持つ値を探索する関数
	引数1: const std::string& key キー名
	返り値: token_array_t 値を表すトークンの集合
	例外: 指定したキーがファイルになかったとき
*/
token_array_t CCsvSettingFileReader::search_value(const std::string& key) const
{
	auto it = m_key_value.find(key);
	if (it == m_key_value.end()) {
		throw CHeisClientException("キーが見つかりませんでした(キー名: %s)", key.c_str());
	}
	return it->second;
}

/*
	カンマの前後にある空白類文字を削除する関数
	参照1: token_array_t& key_value キーと値全体(カンマによってトークンに分割済み)
	返り値なし
*/
void CCsvSettingFileReader::remove_space_around_comma(token_array_t& key_value) const
{
	for (auto& token : key_value) {
		// 単語の先頭と末尾についている空白を削除する
		token = std::regex_replace(token, std::regex("(^\\s+)|(\\s+$)"), "");
	}
}