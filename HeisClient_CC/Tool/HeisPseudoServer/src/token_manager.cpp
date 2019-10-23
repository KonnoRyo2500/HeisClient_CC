// heisクライアント用JSON送受信治具 トークン処理クラス
// Author: Ryo Konno

#include "token_manager.h"

/* public関数 */

/*
	文字列をトークン列に分割する関数
	引数1: const std::string& str 分割元の文字列
	返り値: std::vector<std::string> トークン列
*/
std::vector<std::string> split_string(const std::string& str) const
{
	
}

/*
	トークン列から，指定したインデックスのトークンを1つ取得する関数
	引数1: const std::vector<std::string>& tokens トークン列
	引数2: const int index インデックス
	返り値: std::string トークン
*/
std::string get_single_token(const std::vector<std::string>& tokens, const int index) const
{
	
}

/*
	トークン列から，指定した範囲の各トークンをトークン列として取得する関数
	引数1: const std::vector<std::string>& tokens トークン列
	引数2: const size_t begin_pos 取得範囲の始端
	引数3: const size_t end_pos 取得範囲の終端
	返り値: std::vector<std::string> トークン列
*/
std::vector<std::string> get_token_range(const std::vector<std::string>& tokens, const size_t begin_pos, const size_t end_pos) const
{
	
}

/*
	トークン列から，指定した範囲の各トークンを1つの文字列に連結して取得する関数
	引数1: const std::vector<std::string>& tokens トークン列
	引数2: const size_t begin_pos 取得範囲の始端
	引数3: const size_t end_pos 取得範囲の終端
	返り値: std::string トークンを連結した文字列
*/
std::string get_catnated_tokens(const std::vector<std::string>& tokens, const size_t begin_pos, const size_t end_pos) const
{
	
}

/* private関数 */

/*
	文字列中の制御文字を削除する関数
	参照1: std::string& str 制御文字を削除する対象の文字列
	返り値なし
*/
void CTokenManager::erase_control_letter(std::string& str) const
{
	
}

/*
	文字列の先頭にある区切り文字列を削除する関数
	参照1: std::string& str 区切り文字列を削除する対象の文字列
	引数1: const std::string& delim 区切り文字
	返り値なし
*/
void CTokenManager::erase_first_delimiters(std::string& str, const std::string& delim) const
{
	// 次に出現するトークンの先頭位置
	size_t next_token_pos = str.find_first_not_of(delim);

	if(next_token_pos == std::string::npos){
		// 区切り文字列の後ろにトークンがなければ，分割終了
		str.clear();
		return;
	}
	str = str.substr(next_token_pos);
}

/*
	文字列の先頭にあるトークンを削除する関数
	参照1: std::string& str トークンを削除する対象の文字列
	引数1: const std::string& delim 区切り文字
	返り値なし
*/
void CTokenManager::erase_first_token(std::string& str, const std::string& delim) const
{
	// 次に出現する区切り文字列の先頭位置
	size_t next_delims_pos = str.find_first_of(delim);
	
	if(next_delims_pos == std::string::npos){
		// トークンの後ろに区切り文字列がなければ，そのトークンを削除し分割終了
		str.clear();
	}
	str = str.substr(next_delims_pos);
}

/*
	文字列の先頭にあるトークンを取得する関数
	参照1: std::string& str トークンを削除する対象の文字列
	引数1: const std::string& delim 区切り文字
	返り値: std::string 取得したトークン
*/
std::string CTokenManager::get_first_token(const std::string& str, const std::string& delim) const
{
	// 次に出現する区切り文字列の先頭位置
	size_t next_delims_pos = str.find_first_of(delim);
	
	if(next_delims_pos == std::string::npos){
		// トークンの後ろに区切り文字列がなければ，文字列全体がトークンになる
		return str;
	}
	return str.substr(0, next_delims_pos);
}
