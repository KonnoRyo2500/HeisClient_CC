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
			//! 情報(コンソールに表示する)
			LogLevel_VisibleInfo,
			//! 情報(コンソールン表示しない)
			LogLevel_InvisibleInfo,
			//! 警告
			LogLevel_Warning,
			//! エラー
			LogLevel_Error,
		};

	// メンバ関数
	public:
		// コンストラクタ
		CLog(const std::string& title);
		// デストラクタ
		~CLog();

		// ログに書き込む
		void write_log(const LogLevel level, const std::string& msg) const;

	private:
		// 現在日時を表す文字列を取得
		std::string make_current_datetime_str(const std::string& format) const;

		// ログの種類を表す文字列を作成する
		std::string make_log_level_str(const LogLevel level) const;

	// メンバ変数
	private:
		//! ログファイルの実体
		std::ofstream* m_logfile;
};