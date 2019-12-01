// CSV設定ファイルクラス
// Author: Ryo Konno

#include "csv_setting_file_reader.h"

#include <regex>

/* public関数 */
/*
	コンストラクタ
	引数1: const std::string& file_name 設定ファイル名
	例外: ファイルオープンに失敗したとき
*/
CCsvSettingFileReader::CCsvSettingFileReader(const std::string& file_name)
	:m_file(file_name)
{
	if (m_file.fail()) {
		throw CHeisClientException("設定ファイルのオープンに失敗しました(ファイル名: %s)", file_name.c_str());
	}
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
	指定されたキーの値をCSV設定ファイルからすべて読み込む関数
	引数1: const std::string& key キー名
	返り値: token_array_t 値を表すトークンの集合
	例外: 指定したキーがファイルになかったとき
*/
token_array_t CCsvSettingFileReader::read_all_value(const std::string& key)
{
	std::string str;
	token_array_t key_value;
	CTokenManager tm;

	// キーを検索
	while (std::getline(m_file, str)) {
		key_value = tm.split_string(str, ",");
		remove_space_around_comma(key_value);
		if (key_value.size() > 0 && key_value[0] == key) {
			key_value.erase(key_value.begin());
			// 次回この関数を呼び出したときにファイルを先頭から読み込めるよう，読み出し位置を戻す
			m_file.seekg(0, std::ios_base::beg);
			// キー(先頭要素)を削除しているので，実際に返しているのは値のみ
			return key_value;
		}
	}

	// キー検索失敗
	// 次回この関数を呼び出したときにファイルを先頭から読み込めるよう，読み出し位置を戻す
	m_file.seekg(0, std::ios_base::beg);
	throw CHeisClientException("キーが見つかりませんでした(キー名: %s)", key.c_str());
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
		printf("token: %s\n", token.c_str());
	}
}