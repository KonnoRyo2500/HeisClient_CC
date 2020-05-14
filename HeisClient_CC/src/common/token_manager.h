/**
*	@file		token_manager.h
*	@brief		heis トークン処理
*	@author		Ryo Konno
*	@details	文字列中のトークンを処理するための操作を提供する．
*/
#pragma once

#include <string>
#include <vector>

/*
	MEMO: 用語集
	区切り文字(delimiter): 文字列を区切るための文字の集合
	区切り文字列(delimiters): 区切り文字に含まれる任意の文字がが1文字以上連続で並んだ文字列
	トークン(token): 文字列を区切り文字によって区切った際の各部分文字列
	トークン列(tokens, token array): 文字列から生成されたすべてのトークンを，1つのデータとしてまとめたもの
*/

/* 型定義 */
/**
*	@typedef token_t
*	@brief トークン型
*/
typedef std::string token_t;
/**
*	@typedef token_array_t
*	@brief トークン列型
*/
typedef std::vector<token_t> token_array_t;

// 文字列からトークン列生成
token_array_t split_string(const std::string& str, const std::string& delim);
