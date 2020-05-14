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
	*/
	template <typename ... Args>
	std::string format(const std::string& fmt, Args ... args)
	{
		// snprintf関数の返り値は、第一、第二引数に依らずfmtを展開した後の文字数と同等になる(NULL文字の分は含まれない)
		// したがって、このようにsnprintfを呼ぶことで展開後の文字列の長さが得られる
		size_t len = std::snprintf(NULL, 0, fmt.c_str(), args ...) + 1;
		std::vector<char> fmt_expand(len);

		// vectorはメモリの連続性が保証されている
		std::snprintf(&fmt_expand[0], len, fmt.c_str(), args ...);
		return std::string(&fmt_expand[0]);
	}
}