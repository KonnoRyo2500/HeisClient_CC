// CSV設定ファイルクラス
// Author: Ryo Konno
#pragma once

#include <fstream>
#include <vector>

#include "token_manager.h"
#include "heis_client_exception.h"

// TODO: このクラスを共通ソースに移動させる
class CCsvSettingFileReader {
	// 構造体，列挙体など
	public:
		
	// メンバ関数
	public:
		// コンストラクタ
		explicit CCsvSettingFileReader(const std::string& file_name);

		// デストラクタ
		~CCsvSettingFileReader();

		// 値の取得
		// TODO: 処理を見直す(よりきれいな処理にできないか?)
		template <typename T>
		T get_single_value(const std::string& key, const size_t index);
		template <typename T>
		std::vector<T> get_all_value(const std::string& key);
		template <typename T>
		std::vector<T> get_ranged_value(const std::string& key, const size_t begin_index, const size_t end_index);

	private:
		// 値の読み込み
		token_array_t read_all_value(const std::string& key);
		//不要文字の削除
		void remove_space_around_comma(token_array_t& key_value) const;

	// メンバ変数
	private:
		// CSV設定ファイル
		std::ifstream m_file;
};

// テンプレート関数
/* public関数 */

/*
	単一の値を取得する関数
	引数1: const std::string& key キー名
	引数2: const size_t index 値のインデックス
	返り値: T 値
	備考: 値を文字列として返す場合は別途処理を定義しているため，この処理は数値を返すことを想定している
*/
template <typename T>
T CCsvSettingFileReader::get_single_value(const std::string& key, const size_t index)
{	
	// std::string型のデータを返すget_single_valueは自身を呼び出さない別処理なので，再帰呼び出しにはならない(以下同様)
	std::string value_str = get_single_value<std::string>(key, index);
	/* 
		整数型と浮動小数点型どちらを返すかによって文字列 -> 数値に変換する関数が異なる
		それぞれの型について関数を定義することを防ぐため，一旦浮動小数点に変換し，そのあと必要な型にキャストするようにする
		以下に定義する関数も同様
	*/
	T value = static_cast<T>(stof(value_str));

	return value;
}

/*
	すべての値を取得する関数
	引数1: const std::string& key キー名
	返り値: std::vector<T> 値
*/
template <typename T>
std::vector<T> CCsvSettingFileReader::get_all_value(const std::string& key)
{
	std::vector<std::string> value_str = read_all_value(key);
	std::vector<T> ret_value;

	for (auto& v : value_str) {
		ret_value.push_back(static_cast<T>(stof(v)));
	}

	return ret_value;
}

/*
	指定した範囲のインデックスを持つ値を取得する関数
	引数1: const std::string& key キー名
	引数2: const size_t begin_index 範囲の開始インデックス
	引数3: const size_t end_index 範囲の終了インデックス
	返り値: std::vector<T> 値
*/
template <typename T>
std::vector<T> CCsvSettingFileReader::get_ranged_value(const std::string& key, const size_t begin_index, const size_t end_index)
{
	std::vector<std::string> value_str = get_ranged_value<std::string>(key, begin_index, end_index);
	std::vector<T> ret_value;

	for (auto& v : value_str) {
		ret_value.push_back(static_cast<T>(stof(v)));
	}

	return ret_value;
}

// テンプレート関数特殊化

/*
	単一の値を取得する関数(返す値の型が文字列の場合の処理)
	引数1: const std::string& key キー名
	引数2: const size_t index 値のインデックス
	返り値: std::string 値
	例外: インデックスが範囲外の場合
*/
template<>
inline std::string CCsvSettingFileReader::get_single_value<std::string>(const std::string& key, const size_t index)
{
	token_array_t value = read_all_value(key);
	if (index < value.size()) {
		return value[index];
	}

	throw CHeisClientException("インデックスが範囲外です(インデックスの上限: %d, 指定されたインデックス: %d)", value.size() - 1, index);
}

/*
	すべての値を取得する関数(返す値の型が文字列の場合の処理)
	引数1: const std::string& key キー名
	返り値: std::vector<std::string> 値
*/
template<>
inline std::vector<std::string> CCsvSettingFileReader::get_all_value(const std::string& key)
{
	return read_all_value(key);
}

/*
	指定した範囲のインデックスを持つ値を取得する関数(返す値の型が文字列の場合の処理)
	引数1: const std::string& key キー名
	引数2: const size_t begin_index 範囲の開始インデックス
	引数3: const size_t end_index 範囲の終了インデックス
	返り値: std::vector<std::string> 値
	例外1: 開始インデックス > 終了インデックスの場合
	例外2: インデックスが範囲外の場合
*/
template<>
inline std::vector<std::string> CCsvSettingFileReader::get_ranged_value(const std::string& key, const size_t begin_index, const size_t end_index)
{
	std::vector<std::string> all_value = read_all_value(key);
	std::vector<std::string> ret_value;

	if (begin_index > end_index) {
		throw CHeisClientException("開始インデックスが終了インデックスよりも後ろにあります(開始: %d, 終了: %d)", begin_index, end_index);
	}
	if (begin_index >= all_value.size()) {
		throw CHeisClientException("インデックスが範囲外です(値の個数: %d, 開始: %d, 終了: %d)", all_value.size(), begin_index, end_index);
	}

	for (size_t i = begin_index; i <= end_index; i++) {
		if (i == all_value.size()) {
			break;
		}
		ret_value.push_back(all_value[i]);
	}

	return ret_value;
}