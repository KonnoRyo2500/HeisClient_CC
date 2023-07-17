/**
*	@file		log.cpp
*	@brief		heis ログ出力クラス
*	@author		Ryo Konno
*	@details	ログを出力するための操作を提供する．
*/

#include "log.h"
#include "path.h"

#include <assert.h>
#include <filesystem>

// staticメンバ変数の定義
std::ofstream *CLog::m_log = NULL;

/* public関数 */

/**
*	@brief ログ記録を開始する
*/
void CLog::start_logging()
{
	// 現在日時がログファイル名に含まれるため、既存のログファイルと名前が被ってしまうことは現実的に起こりえない
	std::string log_file_name = "log" + build_current_datetime_str("_%Y_%m_%d_%H_%M_%S") + ".log";
	std::string log_path = join({ CC_PROJECT_DIR, "log", log_file_name });

	// ログファイルの作成
	m_log = new std::ofstream(log_path, std::ios::app);
	if (m_log->fail()) {
		throw std::runtime_error("ログ記録の開始に失敗しました");
	}
}

/**
*	@brief ログ記録を終了する
*/
void CLog::end_logging()
{
	delete m_log;
	m_log = NULL;
}

/**
*	@brief ログにメッセージを追加する
*	@param[in] level ログのレベル(重要度)
*	@param[in] msg 追加するメッセージ
*	@param[in] output_to_console ログメッセージをコンソールに表示するか(デフォルトではfalse(無効))
*/
void CLog::write(LogLevel level, const std::string& msg, bool output_to_console)
{
	// ログ記録が開始されていない
	assert(m_log != NULL);

	// ログのヘッダを作成する
	std::string log_header = "";
	// 書き込み日時
	log_header += build_current_datetime_str("%Y/%m/%d %H:%M:%S ");

	// ログレベル
	switch (level)
	{
	case CLog::LogLevel_Information:
		log_header += "[情報]";
		break;
	case CLog::LogLevel_Warning:
		log_header += "[警告]";
		break;
	case CLog::LogLevel_Error:
		log_header += "[エラー]";
		break;
	default:
		log_header += "[未定義!]";
		break;
	}

	// ログに追記する
	*m_log << log_header << " " << msg << std::endl;

	// 必要があれば、メッセージをコンソールにも出力する
	// ヘッダ部分は
	if (output_to_console) {
		printf("%s\n", msg.c_str());
	}
}

/**
*	@brief 現在日時を表す文字列を作成する
*	@param[in] format 日時を指定するフォーマット文字列(strftime形式)
*	@return std::string 現在日時を表す文字列
*/
std::string CLog::build_current_datetime_str(const std::string& format)
{
	char datetime[200] = { 0 };
	std::time_t currnt_time = std::time(nullptr);

	if (strftime(datetime, sizeof(datetime), format.c_str(), std::localtime(&currnt_time)) == 0) {
		throw std::runtime_error("ログ用の現在日時の取得に失敗しました");
	}
	return std::string(datetime);
}