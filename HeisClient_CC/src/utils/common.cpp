/**
*	@file		common.cpp
*	@brief		heis 共通処理
*	@author		Ryo Konno
*	@details	CC内で共通して使用する処理を定義する．
*/

#include <climits>
#ifdef WIN32
#include <Windows.h>
#else
#include <unistd.h>
#endif // WIN32
#include <stdexcept>

#include "common.h"

/* static関数 */
//! 制御文字の削除
static void erase_control_letter(std::string& str);
//! 先頭区切り文字の削除
static void erase_first_delimiters(std::string& str, const std::string& delim);
//! 先頭トークンの削除
static void erase_first_token(std::string& str, const std::string& delim);
//! 先頭トークンの取得
static std::string get_first_token(const std::string& str, const std::string& delim);
//! 部分文字列削除
static void erase_substring(std::string& str, const std::string& erase_str);

/**
*	@brief 文字列をトークン列に分割する関数
*	@param[in] str 分割元の文字列
*	@param[in] delim 区切り文字
*	@return std::vector<std::string> トークン列
*/
std::vector<std::string> cc_common::split_string(const std::string& str, const std::string& delim)
{
	// 元の文字列は残しておきたいため，文字列の複製をここで作っておく
	std::string str_work(str);
	std::vector<std::string> tokens;

	erase_control_letter(str_work);
	while (str_work.size() > 0) {
		std::string token;

		// 区切り文字を削除する
		erase_first_delimiters(str_work, delim);

		// トークンを1つ切り出す
		token = get_first_token(str_work, delim);
		erase_first_token(str_work, delim);

		// トークン列に追加
		if (token.size() > 0) {
			tokens.push_back(token);
		}
	}
	return tokens;
}

/**
*	@brief 特定の範囲の文字列を切り取って返す関数
*	@param[in] str 文字列
*	@param[in] begin_pos 切り取る範囲の始端位置(0始まり)
*	@param[in] end_pos 切り取る範囲の終端位置(0始まり, 省略可。省略したり、strの終端より後ろを指定した場合はstrの終端)
*/
std::string cc_common::cut_string(std::string& str, const size_t begin_pos, const size_t end_pos)
{
	// 引数のチェック
	if (begin_pos >= str.size()) {
		throw std::runtime_error(cc_common::format("切り取りの開始位置が文字列中にありません(文字列の長さ: %zu, 開始位置: %zu)", str.size(), begin_pos));
	}
	if (begin_pos > end_pos) {
		throw std::runtime_error(cc_common::format("開始位置が終端位置よりも後ろにあります(開始位置: %zu, 終端位置: %zu)", begin_pos, end_pos));
	}

	// 文字列の切り取り
	std::string ret;
	if (end_pos >= str.size()) {
		ret = str.substr(begin_pos);
		str = str.substr(0, begin_pos);
	}
	else {
		ret = str.substr(begin_pos, end_pos - begin_pos + 1);
		str = str.substr(0, begin_pos) + str.substr(end_pos + 1);
	}

	return ret;
}

/* 非公開関数 */
/**
*	@brief 文字列中の制御文字を削除する関数
*	@param[out] str 制御文字を削除する対象の文字列
*/
static void erase_control_letter(std::string& str)
{
	// 削除する制御文字一覧
	const std::vector<std::string> ctl_code_list = {
		// BOM(UTF-8)
		/*
		  この処理により，SJISで「*ｻｿ」(*は2バイト目が0xEFの文字)を含む文字列は不正な文字列になるが，
		  そのような文字列を含むアクションはユーザが運用によりシナリオファイルに記述しないものとする．
		*/
		"\xEF\xBB\xBF",
		// CR
		"\r",
		// LF
		"\n",
	};

	for (auto& ctl_code : ctl_code_list) {
		erase_substring(str, ctl_code);
	}
}

/**
*	@brief 文字列の先頭にある区切り文字列を削除する関数
*	@param[out] str 区切り文字列を削除する対象の文字列
*	@param[in] delim 区切り文字
*/
static void erase_first_delimiters(std::string& str, const std::string& delim)
{
	// 次に出現するトークンの先頭位置
	size_t next_token_pos = str.find_first_not_of(delim);

	if (next_token_pos == std::string::npos) {
		// 区切り文字列の後ろにトークンがなければ，分割終了
		str.clear();
		return;
	}
	str = str.substr(next_token_pos);
}

/**
*	@brief 文字列の先頭にあるトークンを削除する関数
*	@param[out] str トークンを削除する対象の文字列
*	@param[in] delim 区切り文字
*/
static void erase_first_token(std::string& str, const std::string& delim)
{
	// 次に出現する区切り文字列の先頭位置
	size_t next_delims_pos = str.find_first_of(delim);

	if (next_delims_pos == std::string::npos) {
		// トークンの後ろに区切り文字列がなければ，そのトークンを削除し分割終了
		str.clear();
		return;
	}
	str = str.substr(next_delims_pos);
}

/**
*	@brief 文字列の先頭にあるトークンを取得する関数
*	@param[out] str トークンを削除する対象の文字列
*	@param[in] delim 区切り文字
*	@return std::string 取得したトークン
*/
static std::string get_first_token(const std::string& str, const std::string& delim)
{
	// 次に出現する区切り文字列の先頭位置
	size_t next_delims_pos = str.find_first_of(delim);

	if (next_delims_pos == std::string::npos) {
		// トークンの後ろに区切り文字列がなければ，文字列全体がトークンになる
		return str;
	}
	return str.substr(0, next_delims_pos);
}

/**
*	@brief 指定された文字列を，文字列中から削除する関数
*	@param[out] str 加工対象の文字列
*	@param[in] erase_str 削除する文字列
*/
static void erase_substring(std::string& str, const std::string& erase_str)
{
	// TODO: replaceを使えれば，この関数は不要になるかも?
	size_t substr_pos = str.find(erase_str);
	while (substr_pos != std::string::npos) {
		str.erase(substr_pos, erase_str.size());
		substr_pos = str.find(erase_str);
	}
}