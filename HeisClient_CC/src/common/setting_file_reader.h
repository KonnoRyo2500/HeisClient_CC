/**
*	@file		setting_file_reader.h
*	@brief		設定ファイル読み出しクラス
*	@author		Ryo Konno
*	@details	設定ファイルから設定値を読み出す．
*/
#pragma once

#include <map>
#include <stdexcept>

#include "common.h"

/**
*	@brief	設定値読み出しクラス
*/
class CSettingFileReader {
	// 構造体，列挙体など
	public:
		
	// メンバ関数
	public:
		// コンストラクタ
		explicit CSettingFileReader(const std::string& path);

		// デストラクタ
		~CSettingFileReader();

		// 値の取得
		template <typename T>
		T get_value(const std::string& key) const;
		// キーの検索
		bool exists_key(const std::string& key) const;

	private:
		// 値の取得
		void load(const std::string& path);

	// メンバ変数
	private:
		//! 設定ファイルから取得したキーと値の組
		std::map<std::string, std::string> m_key_value;
		//! 設定ファイルのパス
		std::string m_path;
};

// テンプレート関数
/* public関数 */

/**
*	@brief 値を取得する関数
*	@param[in] key キー名
*	@return T 値
*	@remark 値を文字列として返す場合は別途処理を定義しているため，この処理は数値を返すことを想定している
*/
template <typename T>
T CSettingFileReader::get_value(const std::string& key) const
{	
	// 文字列での値取得は別途処理を定義しているため、再帰処理にならない
	// また、キーの存在チェックも行われる
	std::string value_str = get_value<std::string>(key);
	/* 
		整数型と浮動小数点型どちらを返すかによって文字列 -> 数値に変換する関数が異なる
		それぞれの型について関数を定義することを防ぐため，一旦浮動小数点に変換し，そのあと必要な型にキャストするようにする
	*/
	T value = static_cast<T>(stof(value_str));
	return value;
}

// テンプレート関数特殊化

/**
*	@brief 値を取得する関数(返す値の型が文字列の場合の処理)
*	@param[in] key キー名
*	@return std::string 値
*	@throws std::runtime_error キーが存在しない場合
*/
template<>
inline std::string CSettingFileReader::get_value(const std::string& key) const
{
	if (!exists_key(key)) {
		throw std::runtime_error(cc_common::format("キー%sがファイル%sに存在しません", key.c_str(), m_path.c_str()));
	}
	// []演算子はconstな関数でないため、使用できない
	return m_key_value.at(key);
}