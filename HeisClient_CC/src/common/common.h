/**
*	@file		common.h
*	@brief		heis 共通処理
*	@author		Ryo Konno
*	@details	CC内で共通して使用する処理を定義する．
*/
#pragma once



#include <string>
#include <vector>

//! 共通処理・データ構造
namespace cc_common {
	/* パス操作 */
	// パスの区切り文字を取得
	char get_separator_char();
	// 設定ファイルのあるディレクトリを取得
	std::string get_setting_dir();
	// ログファイルのあるディレクトリを取得
	std::string get_log_dir();

	/* 文字列処理 */
	// 文字列の分割
	std::vector<std::string> split_string(const std::string& str, const std::string& delim);
	// 文字列の切り取り
	std::string cut_string(std::string& str, const size_t begin_pos, const size_t end_pos = SIZE_MAX);

	/* テンプレート関数 */
	/**
	*	@brief 相対パスを作成する関数
	*	@details パスの区切り文字は、実行環境に準ずる
	*	@remark 引数に与える各ディレクトリの末尾には、区切り文字をつけないこと
	*	@param[in] base_dir 基準となるディレクトリ
	*	@param[in] contents 途中で経由するディレクトリ名や対象のファイル名
	*	@return std::string 相対パス
	*/
	template<typename ... Args>
	std::string make_relative_path(const std::string& base_dir, Args ... contents)
	{
		std::string relative_path = base_dir;

		for (auto content : std::initializer_list<std::string>{ contents... }) {
			relative_path += get_separator_char() + content;
		}

		return relative_path;
	}
	
	/**
	*	@brief 特定の文字や文字列が含まれているか判定する関数
	*	@param[in] str 文字列
	*	@param[in] target 検索する文字 or 文字列
	*	@return bool strにtargetが含まれているか(含まれていればtrue)
	*/
	template<typename T>
	bool contains(const std::string& str, const T& target)
	{
		return str.find(target) != std::string::npos;
	}

	/**
	*	@brief フォーマット文字列を展開する関数
	*	@param[in] fmt フォーマット文字列
	*	@param[in] args fmtに与える引数
	*	@return std::string fmtを展開した文字列
	*/
	template<typename ... Args>
	std::string format(const std::string& fmt, Args ... args)
	{
		// 展開後の文字列長を得る(+1はNULL文字の分)
		size_t expand_len = std::snprintf(NULL, 0, fmt.c_str(), args ...) + 1;
		std::vector<char> expand_str(expand_len);

		std::snprintf(&expand_str[0], expand_len, fmt.c_str(), args ...);
		return std::string(&expand_str[0]);
	}
}