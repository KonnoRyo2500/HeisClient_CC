/**
*	@file		log.cpp
*	@brief		heis ログ出力クラス
*	@author		Ryo Konno
*	@details	ログを出力するための操作を提供する．
*/
#include "log.h"
#include "path_generator.h"
#include "common.h"

#include <ctime>
#include <locale>
#include <iostream>
#include <map>
#include <cstdarg>
#include <vector>

/* public関数 */

/**
*	@brief コンストラクタ
*	@param[in] log_title ログファイルのタイトル
*	@param[in] add_datetime_to_name ログファイル名の末尾に現在日時をつけるか(デフォルトはtrue)
*	@remark ログファイル名は"(タイトル)_(日付).log"もしくは"(タイトル).log"となる
*/
CLog::CLog(const std::string& log_title, const bool add_datetime_to_name)
{
	// "bin"ディレクトリと同列の"log"ディレクトリにログを出力する
	// WindowsとLinuxでプロジェクトディレクトリの構造が異なるので，それに合わせてログ出力先の
	// パスも変える
	std::string log_name = get_log_dir() + log_title;

	if (add_datetime_to_name) {
		log_name += make_current_datetime_str("_%Y_%m_%d_%H_%M_%S");
	}
	log_name += ".log";

	m_logfile = new std::ofstream(log_name);
	if (m_logfile->fail()) {
		throw std::runtime_error(cc_common::format("ログファイルのオープンに失敗しました(ファイル名: %s)", log_name.c_str()));
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
*	@param[in] log_type ログの種類
*	@param[in] visible ログと同じ文字列を，コンソールにも表示するか
*	@param[in] format 書き込むメッセージ(フォーマット文字列可)
*	@param[in] ... フォーマット文字列の引数
*/
void CLog::write_log(const LogType log_type, const bool visible, const char* format, ...) const
{
	va_list args;

	va_start(args, format);
	int message_len = vsnprintf(NULL, 0, format, args);

	std::vector<char> message_buf(message_len + 1);
	vsnprintf(&message_buf[0], message_len + 1, format, args);
	va_end(args);

	const std::string log_content = make_current_datetime_str("%Y/%m/%d %H:%M:%S ")
		+ "["
		+ make_log_type_str(log_type)
		+ "] "
		+ std::string(message_buf.data());

	*m_logfile << log_content << std::endl;
	if (visible) {
		printf("%s\n", message_buf.data());
	}
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
*	@brief ログの種類を表す文字列を作成する関数
*	@param[in] type ログの種類
*	@return std::string ログの種類を表す文字列
*/
std::string CLog::make_log_type_str(const LogType type) const
{
	// ログの種類 -> 文字列の対応表
	const std::map<LogType, std::string> log_type_map = {
		{LogType_Infomation,	"情報"},
		{LogType_Warning,		"警告"},
		{LogType_Error,			"エラー"},
	};

	auto it = log_type_map.find(type);
	if (it != log_type_map.end()) {
		return it->second;
	}

	return "未定義のログ";
}