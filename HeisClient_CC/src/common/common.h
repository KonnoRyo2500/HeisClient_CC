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

	/* テンプレート関数 */
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