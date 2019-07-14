// heis 例外クラス
// Author: Ryo Konno
#pragma once

#include <stdexcept>

class CHeisClientException : public std::runtime_error
{
	// メンバ関数
	public:
		// コンストラクタ
		explicit CHeisClientException(const char* format, ...);

		// 内容を表示
		const char* what() const override;

	// メンバ変数
	private:
		// 例外発生時のエラーメッセージ
		std::string m_error_message;
};