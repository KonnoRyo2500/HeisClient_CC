/**
*	@file		string_util.cpp
*	@brief		heis 文字列処理クラス
*	@author		Ryo Konno
*	@details	文字列に関する各種処理を行うユーティリティクラス。
*/

#include "string_utils.h"

/**
*	@brief 文字列を特定の文字で区切る
*	@param[in] src 文字列
*	@param[in] delimiter 区切り文字
*	@return std::vector<std::string> 分割後の文字列
*/
std::vector<std::string> CStringUtil::split(const std::string& src, const char delimiter)
{
	std::vector<std::string> tokens;

	int prev_delimiter_idx = -1;
	for (int i = 0; i < src.size(); i++) {
		if (src[i] != delimiter && i < src.size() - 1) {
			continue;
		}

		// 区切り文字(or srcの末尾)に到達したら、前の区切り文字(or srcの先頭)から現在の区切り文字間にある文字列をtokensに追加する
		int token_last_char_idx = (src[i] != delimiter && i == src.size() - 1 ? i : i - 1);
		int token_len = token_last_char_idx - prev_delimiter_idx;

		if (token_len == 0) {
			prev_delimiter_idx = i;
			continue;
		}
		
		std::string token = src.substr(prev_delimiter_idx + 1, token_len);
		tokens.push_back(token);

		prev_delimiter_idx = i;
	}

	return tokens;
}