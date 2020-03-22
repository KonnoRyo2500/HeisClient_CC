/**
*	@file		heis_client_exception.cpp
*	@brief		heis 例外クラス
*	@author		Ryo Konno
*	@details	CC内で投げる例外を定義する．
*/
#include "heis_client_exception.h"
#include <cstdarg>
#include <vector>


/**
*	@brief コンストラクタ
*	@param[in] format フォーマット文字列
*	@param[in] ... フォーマット文字列の引数
*/
CHeisClientException::CHeisClientException(const char* format, ...)
	: std::runtime_error(format)
{
	va_list args;

	va_start(args, format);
	// フォーマット文字列を変換した後の文字数を取得
	// vsnprintfは書き込む予定だった文字列長を返すので，以下のような呼び方をすれば変換後の文字列長だけ取得できる
	int error_message_len = vsnprintf(NULL, 0, format, args);

	// std::stringはvsnprintfの引数として使えない
	// 文字列バッファの大きさは変数error_message_lenによって決まるので，charの配列も使えない
	// なので，charのvectorを一旦バッファとして用いる
	std::vector<char> error_message_buf(error_message_len + 1);
	vsnprintf(&error_message_buf[0], error_message_len + 1, format, args);
	va_end(args);

	// 文字列の取り扱いが簡単なstd::stringに変換
	m_error_message = std::string(error_message_buf.data());
}

/**
*	@brief 例外の詳細を表示する関数
*	@return const char* 例外の内容
*/
const char* CHeisClientException::what() const noexcept
{
	return m_error_message.c_str();
}