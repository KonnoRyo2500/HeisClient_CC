/**
*	@file		log.h
*	@brief		heis ログ出力クラス
*	@author		Ryo Konno
*	@details	ログを出力するための操作を提供する．
*/
#pragma once

#include <string>
#include <fstream>

/**
*	@brief	ログ出力クラス
*	@remark インスタンスを作成しなくてもログ記録ができるよう、staticクラスとする
*/
class CLog
{
	// 構造体，列挙体など
	public:
		/**
		*	@enum LogLevel
		*	ログの種類
		*/
		enum LogLevel {
			//! 情報
			LogLevel_Information,
			//! 警告
			LogLevel_Warning,
			//! エラー
			LogLevel_Error,
		};

	// クラス関数
public:
	// ログ記録を開始する
	static void start_logging();
	// ログ記録を終了する
	static void end_logging();
	// ログにメッセージを追加する
	static void write(LogLevel level, std::string msg, bool output_to_console = false);

private:
	// 現在日時を表す文字列を作成する
	static std::string build_current_datetime_str(const std::string& format);

	// クラス変数
private:
	static std::ofstream *m_log;
};