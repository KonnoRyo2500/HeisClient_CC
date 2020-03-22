/**
*	@file		heis_client_exception.h
*	@brief		heis 例外クラス
*	@author		Ryo Konno
*	@details	CC内で投げる例外を定義する．
*/
#pragma once

#include <stdexcept>

/**
*	@brief	CC例外クラス
*/
class CHeisClientException : public std::runtime_error
{
	// メンバ関数
	public:
		// コンストラクタ
		explicit CHeisClientException(const char* format, ...);

		// 内容を表示
		const char* what() const noexcept override;

	// メンバ変数
	private:
		//! 例外発生時のエラーメッセージ
		std::string m_error_message;
};