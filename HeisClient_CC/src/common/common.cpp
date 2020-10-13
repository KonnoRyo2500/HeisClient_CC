/**
*	@file		common.cpp
*	@brief		heis 共通処理
*	@author		Ryo Konno
*	@details	CC内で共通して使用する処理を定義する．
*/

#include <stdexcept>

#include "common.h"

/**
*	@brief 特定の範囲の文字列を切り取って返す関数
*	@param[in] str 文字列
*	@param[in] 切り取る範囲の始端位置(0始まり)
*	@param[in] 切り取る範囲の終端位置(0始まり, 省略可。省略したり、strの終端より後ろを指定した場合はstrの終端)
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