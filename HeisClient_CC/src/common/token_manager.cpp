// heisクライアント用JSON送受信治具 トークン処理クラス
// Author: Ryo Konno

#include <stdexcept>

#include "token_manager.h"
#include "heis_client_exception.h"

/* public関数 */

/*
	文字列をトークン列に分割する関数
	引数1: const std::string& str 分割元の文字列
	引数2: const std::string& delim 区切り文字
	返り値: token_array_t トークン列
*/
token_array_t CTokenManager::split_string(const std::string& str, const std::string& delim)
{
	// 元の文字列は残しておきたいため，文字列の複製をここで作っておく
	std::string str_work(str);
	token_array_t tokens;
	
	erase_control_letter(str_work);
	while(str_work.size() > 0){
		std::string token;
		
		// 区切り文字を削除する
		erase_first_delimiters(str_work, delim);

		// トークンを1つ切り出す
		token = get_first_token(str_work, delim);
		erase_first_token(str_work, delim);

		// トークン列に追加
		if(token.size() > 0){
			tokens.push_back(token);
		}
	}
	return tokens;
}

/*
	トークン列から，指定したインデックスのトークンを1つ取得する関数
	引数1: const token_array_t& tokens トークン列
	引数2: const int index インデックス
	返り値: token_t トークン
	例外: インデックスが0～tokens.size()-1の範囲外のとき
*/
token_t CTokenManager::get_single_token(const token_array_t& tokens, const int index)
{
	try {
		return tokens.at(index);
	}
	catch (std::out_of_range&) {
		throw CHeisClientException("インデックスが範囲外です(インデックスの上限: %d, 指定されたインデックス: %d)", tokens.size() - 1, index);
	}
	
}

/*
	トークン列から，指定した範囲の各トークンをトークン列として取得する関数
	引数1: const token_array_t& tokens トークン列
	引数2: const size_t begin_pos 取得範囲の始端
	引数3: const size_t end_pos 取得範囲の終端
	返り値: token_array_t トークン列
	例外1: 取得範囲が0～tokens.size()-1の範囲外にはみ出しているとき
	例外2: begin_pos > end_posであるとき
*/
token_array_t CTokenManager::get_token_range(const token_array_t& tokens, const size_t begin_pos, const size_t end_pos)
{
	if(begin_pos > end_pos){
		throw CHeisClientException("開始インデックスが終了インデックスよりも後ろにあります(開始: %d, 終了: %d)", begin_pos, end_pos);
	}

	try {
		token_array_t sub_tokens;
		for (size_t i = begin_pos; i <= end_pos; i++) {
			sub_tokens.push_back(tokens.at(i));
		}
		return sub_tokens;
	}
	catch (std::out_of_range&) {
		throw CHeisClientException("インデックスが範囲外です(値の個数: %d, 開始: %d, 終了: %d)", tokens.size(), begin_pos, end_pos);
	}
}

/*
	トークン列から，指定した範囲の各トークンを1つの文字列に連結して取得する関数
	引数1: const token_array_t& tokens トークン列
	引数2: const size_t begin_pos 取得範囲の始端
	引数3: const size_t end_pos 取得範囲の終端
	返り値: std::string トークンを連結した文字列
	例外: 取得範囲が0～tokens.size()-1の範囲外にはみ出しているとき
*/
std::string CTokenManager::get_catnated_tokens(const token_array_t& tokens, const size_t begin_pos, const size_t end_pos)
{
	token_array_t sub_tokens = get_token_range(tokens, begin_pos, end_pos);
	std::string cat_str;

	for(auto& tk : sub_tokens){
		cat_str += tk;
	}

	return cat_str;
}

/* private関数 */

/*
	文字列中の制御文字を削除する関数
	参照1: std::string& str 制御文字を削除する対象の文字列
	返り値なし
*/
void CTokenManager::erase_control_letter(std::string& str)
{
	// 削除する制御文字一覧
	const std::vector<std::string> ctl_code_list = {
		// BOM(UTF-8)
		/*
		  この処理により，SJISで「*ｻｿ」(*は2バイト目が0xEFの文字)を含む文字列は不正な文字列になるが，
		  そのような文字列を含むアクションはユーザが運用によりシナリオファイルに記述しないものとする．
		*/
		"\xEF\xBB\xBF",
		// CR
		"\r",
		// LF
		"\n",
	};

	for(auto& ctl_code : ctl_code_list){
		erase_substring(str, ctl_code);
	}
}

/*
	文字列の先頭にある区切り文字列を削除する関数
	参照1: std::string& str 区切り文字列を削除する対象の文字列
	引数1: const std::string& delim 区切り文字
	返り値なし
*/
void CTokenManager::erase_first_delimiters(std::string& str, const std::string& delim)
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
void CTokenManager::erase_first_token(std::string& str, const std::string& delim)
{
	// 次に出現する区切り文字列の先頭位置
	size_t next_delims_pos = str.find_first_of(delim);
	
	if(next_delims_pos == std::string::npos){
		// トークンの後ろに区切り文字列がなければ，そのトークンを削除し分割終了
		str.clear();
		return;
	}
	str = str.substr(next_delims_pos);
}

/*
	文字列の先頭にあるトークンを取得する関数
	参照1: std::string& str トークンを削除する対象の文字列
	引数1: const std::string& delim 区切り文字
	返り値: token_t 取得したトークン
*/
token_t CTokenManager::get_first_token(const std::string& str, const std::string& delim)
{
	// 次に出現する区切り文字列の先頭位置
	size_t next_delims_pos = str.find_first_of(delim);
	
	if(next_delims_pos == std::string::npos){
		// トークンの後ろに区切り文字列がなければ，文字列全体がトークンになる
		return str;
	}
	return str.substr(0, next_delims_pos);
}

/*
	指定された文字列を，文字列中から削除する関数
	参照1: std::string& str 加工対象の文字列
	引数1: const std::string& erase_str 削除する文字列
	返り値なし
*/
void CTokenManager::erase_substring(std::string& str, const std::string& erase_str)
{
	size_t substr_pos = str.find(erase_str);
	while(substr_pos != std::string::npos){
		str.erase(substr_pos, erase_str.size());
		substr_pos = str.find(erase_str);
	}
}
