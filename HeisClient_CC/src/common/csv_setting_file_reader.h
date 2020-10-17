/**
*	@file		csv_setting_file_reader.h
*	@brief		CSV設定ファイルクラス
*	@author		Ryo Konno
*	@details	CSVファイルとして実現された設定ファイルから設定値を読み出す．
*/
#pragma once

#include <map>
#include <stdexcept>

#include "common.h"

/**
*	@brief	設定値読み出しクラス
*/
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
		T get_single_value(const std::string& key, const size_t index) const;
		template <typename T>
		std::vector<T> get_all_value(const std::string& key) const;

	private:
		// 値の取得
		void load_all_value(const std::string& file_name);

		// 値の検索
		std::vector<std::string> search_value(const std::string& key) const;

		//不要文字の削除
		void remove_space_around_comma(std::vector<std::string>& key_value) const;

	// メンバ変数
	private:
		//! 設定ファイルから取得したキーと値の組
		std::map<std::string, std::vector<std::string>> m_key_value;
};

// テンプレート関数
/* public関数 */

/**
*	@brief 単一の値を取得する関数
*	@param[in] key キー名
*	@param[in] index 値のインデックス
*	@return T 値
*	@remark 値を文字列として返す場合は別途処理を定義しているため，この処理は数値を返すことを想定している
*/
template <typename T>
T CCsvSettingFileReader::get_single_value(const std::string& key, const size_t index) const
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

/**
*	@brief すべての値を取得する関数
*	@param[in] key キー名
*	@return std::vector<T> 値
*/
template <typename T>
std::vector<T> CCsvSettingFileReader::get_all_value(const std::string& key) const
{
	std::vector<std::string> value_str = search_value(key);
	std::vector<T> ret_value;

	for (auto& v : value_str) {
		ret_value.push_back(static_cast<T>(stof(v)));
	}

	return ret_value;
}

// テンプレート関数特殊化

/**
*	@brief 単一の値を取得する関数(返す値の型が文字列の場合の処理)
*	@param[in] key キー名
*	@param[in] index 値のインデックス
*	@return std::string 値
*	@throws std::runtime_error インデックスが範囲外の場合
*/
template<>
inline std::string CCsvSettingFileReader::get_single_value(const std::string& key, const size_t index) const
{
	std::vector<std::string> value = search_value(key);
	try {
		return value.at(index);
	}
	catch (std::out_of_range&) {
		throw std::runtime_error(cc_common::format("インデックスが範囲外です(インデックスの上限: %d, 指定されたインデックス: %d)", value.size() - 1, index));
	}
}

/**
*	@brief すべての値を取得する関数(返す値の型が文字列の場合の処理)
*	@param[in] key キー名
*	@return std::vector<std::string> 値
*/
template<>
inline std::vector<std::string> CCsvSettingFileReader::get_all_value(const std::string& key) const
{
	return search_value(key);
}