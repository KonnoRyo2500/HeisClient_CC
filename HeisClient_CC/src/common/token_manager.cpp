/**
*	@file		token_manager.cpp
*	@brief		heis トークン処理クラス
*	@author		Ryo Konno
*	@details	文字列中のトークンを処理するための操作を提供する．
*/
#include <stdexcept>

#include "token_manager.h"
#include "heis_client_exception.h"

/* public関数 */

/**
*	@brief 文字列をトークン列に分割する関数
*	@param[in] str 分割元の文字列
*	@param[in] delim 区切り文字
*	@return token_array_t トークン列
*/
token_array_t CTokenManager::split_string(const std::string& str, const std::string& delim)
{
	// 元の文字列は残しておきたいため，文字列の複製をここで作っておく
	std::string str_work(str);
	token_array_t tokens;
	
	erase_control_letter(str_work);
	while(str_work.size() > 0){
		std::string token;
		
		// 区切り文字を削除する
		erase_first_delimiters(str_work, delim);

		// トークンを1つ切り出す
		token = get_first_token(str_work, delim);
		erase_first_token(str_work, delim);

		// トークン列に追加
		if(token.size() > 0){
			tokens.push_back(token);
		}
	}
	return tokens;
}

/* private関数 */

/**
*	@brief 文字列中の制御文字を削除する関数
*	@param[out] str 制御文字を削除する対象の文字列
*/
void CTokenManager::erase_control_letter(std::string& str)
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

	for(auto& ctl_code : ctl_code_list){
		erase_substring(str, ctl_code);
	}
}

/**
*	@brief 文字列の先頭にある区切り文字列を削除する関数
*	@param[out] str 区切り文字列を削除する対象の文字列
*	@param[in] delim 区切り文字
*/
void CTokenManager::erase_first_delimiters(std::string& str, const std::string& delim)
{
	// 次に出現するトークンの先頭位置
	size_t next_token_pos = str.find_first_not_of(delim);

	if(next_token_pos == std::string::npos){
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
void CTokenManager::erase_first_token(std::string& str, const std::string& delim)
{
	// 次に出現する区切り文字列の先頭位置
	size_t next_delims_pos = str.find_first_of(delim);
	
	if(next_delims_pos == std::string::npos){
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
*	@return token_t 取得したトークン
*/
token_t CTokenManager::get_first_token(const std::string& str, const std::string& delim)
{
	// 次に出現する区切り文字列の先頭位置
	size_t next_delims_pos = str.find_first_of(delim);
	
	if(next_delims_pos == std::string::npos){
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
void CTokenManager::erase_substring(std::string& str, const std::string& erase_str)
{
	// TODO: replaceを使えれば，この関数は不要になるかも?
	size_t substr_pos = str.find(erase_str);
	while(substr_pos != std::string::npos){
		str.erase(substr_pos, erase_str.size());
		substr_pos = str.find(erase_str);
	}
}
