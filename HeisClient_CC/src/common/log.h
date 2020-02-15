// heis ログ出力クラス
// Author: Ryo Konno
#pragma once

#include <string>
#include <fstream>

class CLog
{
	// 構造体，列挙体など
	public:
		// ログの種類
		enum LogType {
			// 情報
			LogType_Infomation,
			// 警告
			LogType_Warning,
			// エラー
			LogType_Error,
		};

	// メンバ関数
	public:
		// コンストラクタ
		CLog(const std::string& log_name, const bool add_datetime_to_name = true);
		// デストラクタ
		~CLog();

		// ログに書き込む
		void write_log(const LogType log_type, const char* format, ...) const;

	private:
		// 現在日時を表す文字列を取得
		std::string make_current_datetime_str(const std::string& format) const;

		// ログの種類を表す文字列を作成する
		std::string make_log_type_str(const LogType type) const;

	// メンバ変数
	private:
		std::ofstream* m_logfile;
};