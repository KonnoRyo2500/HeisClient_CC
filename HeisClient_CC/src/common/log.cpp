// heis ログ出力クラス
// Author: Ryo Konno
#include "log.h"
#include "heis_client_exception.h"
#include "path_generator.h"

#include <ctime>
#include <locale>
#include <iostream>
#include <map>

/* public関数 */

/*
	コンストラクタ
	引数1: const std::string& log_name ログファイル名
	引数2: const bool add_datetime_to_name ログファイル名の末尾に現在日時をつけるか(デフォルトはtrue)
*/
CLog::CLog(const std::string& log_name, const bool add_datetime_to_name)
{
	// "bin"ディレクトリと同列の"log"ディレクトリにログを出力する
	// WindowsとLinuxでプロジェクトディレクトリの構造が異なるので，それに合わせてログ出力先の
	// パスも変える
#ifdef WIN32
	std::string actual_log_name = CPathGenerator::get_exe_path() + "..\\..\\log\\" + log_name;
#else
	std::string actual_log_name = CPathGenerator::get_exe_path() + "../log/" + log_name;
#endif // WIN32

	if (add_datetime_to_name) {
		actual_log_name += make_current_datetime_str("%Y_%m_%d_%H_%M_%S");
	}
	actual_log_name += ".log";

	m_logfile = new std::ofstream(actual_log_name);
	if (m_logfile->fail()) {
		throw CHeisClientException("ログファイルのオープンに失敗しました(ファイル名: %s)", actual_log_name.c_str());
	}
}

/*
	デストラクタ
	引数なし
*/
CLog::~CLog()
{
	delete m_logfile;
	m_logfile = NULL;
}

/*
	ログにメッセージを書き込む関数
	引数1: const LogType log_type ログの種類
	引数2: const std::string& message 書き込むメッセージ
	返り値なし
*/
void CLog::write_log(const LogType log_type, const std::string& message) const
{
	*m_logfile <<
		make_current_datetime_str("%Y/%m/%d %H:%M:%S ") <<
		"[" + make_log_type_str(log_type) + "] " <<
		message <<
		std::endl;
}

/* private関数 */

/*
	現在日時を表す文字列を返す関数
	引数1: const std::string& format 日時を指定するフォーマット文字列(strftime形式)
	返り値: std::string 現在日時を表す文字列
*/
std::string CLog::make_current_datetime_str(const std::string& format) const
{
	char datetime[200] = { 0 };
	std::time_t currnt_time = std::time(nullptr);

	if (strftime(datetime, sizeof(datetime), format.c_str(), std::localtime(&currnt_time)) == 0) {
		throw CHeisClientException("ログ用の現在日時の取得に失敗しました");
	}
	return std::string(datetime);
}

/*
	ログの種類を表す文字列を作成する関数
	引数1: const LogType type ログの種類
	返り値: std::string ログの種類を表す文字列
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