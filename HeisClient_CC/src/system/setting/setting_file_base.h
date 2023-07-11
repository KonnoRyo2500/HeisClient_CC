/**
*	@file		setting_file_base.h
*	@brief		heis 設定ファイルベースクラス
*	@author		Ryo Konno
*	@details	CSV形式の設定ファイルを読み書きする各種クラスのベースクラス。
*/

#pragma once

#include <string>
#include <map>
#include <vector>
#include <fstream>

#include "common.h"

/**
*	@brief 構造化されたCSVレコードの型
*/
typedef std::map<std::string, std::vector<std::string>> CsvRecords;

/**
*	@brief	設定ファイルベースクラス
*	@tparam SettingType 設定値クラスの型
*/
template <typename SettingType>
class CSettingFileBase
{
	// メンバ関数
public:
	// 設定値をローカルモード設定ファイルから読み込む
	virtual SettingType load(const std::string& path) const = 0;
	// 設定値をローカルモード設定ファイルに書き込む
	virtual void save(const std::string& path, const SettingType& setting) const = 0;

protected:
	// CSVファイルの中身をパースする
	CsvRecords parse_csv(const std::string& path) const;
};

/**
*	@brief CSVファイルをから各行のレコードを読み込み、その中身をパースする
*	@tparam SettingType 設定ファイルクラスの型
*	@param[in] path CSVファイルのパス
*	@return CsvRecords 構造化されたCSVレコード
*/
template <typename SettingType>
CsvRecords CSettingFileBase<SettingType>::parse_csv(const std::string& path) const
{
	std::ifstream in_stream(path);
	CsvRecords records;

	// 設定ファイルの読み込みに失敗した
	if (in_stream.fail()) {
		throw std::runtime_error(cc_common::format("設定ファイル%sのオープンに失敗しました", path.c_str()));
	}

	// 各行をパースする
	std::string csv_line;
	while (std::getline(in_stream, csv_line)) {
		std::vector<std::string> parsed_line = cc_common::split_string(csv_line, ",");

		// キー名と最低1個以上の値が1行に存在しなければエラー
		if (parsed_line.empty()) {
			throw std::runtime_error("設定ファイルに空行があります");
		}
		if (parsed_line.size() == 1) {
			throw std::runtime_error("設定ファイルに値が欠損している行があります");
		}

		// パースした行から得られたキーと値を、構造化済みCSVレコード(records)に追加
		std::string key = parsed_line[0];
		std::vector<std::string> values(parsed_line.size() - 1);
		std::copy(
			parsed_line.begin() + 1,
			parsed_line.end(),
			values.begin()
		);
		records.insert_or_assign(key, values);
	}

	return records;
}