/**
*	@file		string_util.h
*	@brief		heis 文字列処理クラス
*	@author		Ryo Konno
*	@details	文字列に関する各種処理を行うユーティリティクラス。
*/

#pragma once

#include <vector>
#include <string>

/**
*	@brief 文字列処理クラス
*/
class CStringUtil
{
	// メンバ関数
public:
	// 文字列を特定の文字で区切る
	static std::vector<std::string> split(const std::string& src, const char delimiter);
	// フォーマット文字列を展開する
	template<typename ... Args>
	static std::string format(const std::string& fmt, Args ... args);
};

/**
	*	@brief フォーマット文字列を展開する
	*	@param[in] fmt フォーマット文字列
	*	@param[in] args fmtに与える引数
	*	@return std::string fmtを展開した文字列
	*/
template<typename ... Args>
std::string CStringUtil::format(const std::string& fmt, Args ... args)
{
	// 展開後の文字列長を得る(+1はNULL文字の分)
	size_t expand_len = std::snprintf(NULL, 0, fmt.c_str(), args ...) + 1;
	std::vector<char> expand_str(expand_len);

	std::snprintf(&expand_str[0], expand_len, fmt.c_str(), args ...);
	return std::string(&expand_str[0]);
}