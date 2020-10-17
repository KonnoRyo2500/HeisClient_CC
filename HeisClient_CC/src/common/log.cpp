/**
*	@file		log.cpp
*	@brief		heis ログ出力クラス
*	@author		Ryo Konno
*	@details	ログを出力するための操作を提供する．
*/
#include "log.h"
#include "common.h"

#include <ctime>
#include <locale>
#include <iostream>
#include <map>
#include <vector>

/* public関数 */

/**
*	@brief コンストラクタ
*	@param[in] title ログファイルのタイトル
*	@remark ログファイル名は"(タイトル)_(日付).log"となる
*/
CLog::CLog(const std::string& title)
{
	// "bin"ディレクトリと同列の"log"ディレクトリにログを出力する
	std::string log_path =
		cc_common::get_log_dir()
		+ cc_common::get_separator_char()
		+ title
		+ make_current_datetime_str("_%Y_%m_%d_%H_%M_%S")
		+ ".log";

	m_logfile = new std::ofstream(log_path);
	if (m_logfile->fail()) {
		throw std::runtime_error(cc_common::format("ログファイルのオープンに失敗しました(パス: %s)", log_path.c_str()));
	}
}

/**
*	@brief デストラクタ
*/
CLog::~CLog()
{
	delete m_logfile;
	m_logfile = NULL;
}

/**
*	@brief ログにメッセージを書き込む関数
*	@param[in] level ログレベル
*	@param[in] msg 書き込むメッセージ
*/
void CLog::write_log(const LogLevel level, const std::string& msg) const
{
	const std::string log_content = make_current_datetime_str("%Y/%m/%d %H:%M:%S ")
		+ "["
		+ make_log_level_str(level)
		+ "] "
		+ msg;

	*m_logfile << log_content << std::endl;
	
	if (level == LogLevel_InvisibleInfo) {
		return;
	}
	printf("%s\n", msg.c_str());
}

/* private関数 */

/**
*	@brief 現在日時を表す文字列を返す関数
*	@param[in] format 日時を指定するフォーマット文字列(strftime形式)
*	@return std::string 現在日時を表す文字列
*/
std::string CLog::make_current_datetime_str(const std::string& format) const
{
	char datetime[200] = { 0 };
	std::time_t currnt_time = std::time(nullptr);

	if (strftime(datetime, sizeof(datetime), format.c_str(), std::localtime(&currnt_time)) == 0) {
		throw std::runtime_error("ログ用の現在日時の取得に失敗しました");
	}
	return std::string(datetime);
}

/**
*	@brief ログレベルを表す文字列を作成する関数
*	@param[in] level ログレベル
*	@return std::string ログレベルを表す文字列
*/
std::string CLog::make_log_level_str(const LogLevel level) const
{
	// ログレベル -> 文字列の対応表
	const std::map<LogLevel, std::string> log_level_map = {
		{LogLevel_VisibleInfo,		"情報"},
		{LogLevel_InvisibleInfo,	"情報"},
		{LogLevel_Warning,			"警告"},
		{LogLevel_Error,			"エラー"},
	};

	auto it = log_level_map.find(level);
	if (it != log_level_map.end()) {
		return it->second;
	}

	return "未定義のログ";
}