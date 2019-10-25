// heisクライアント用JSON送受信治具 トークン処理クラス
// Author: Ryo Konno

#include <string>
#include <vector>

/*
	MEMO: 用語集
	区切り文字(delimiter): 文字列を区切るための文字の集合
	区切り文字列(delimiters): 区切り文字に含まれる任意の文字がが1文字以上連続で並んだ文字列
	トークン(token): 文字列を区切り文字によって区切った際の各部分文字列
	トークン列(tokens): 文字列から生成されたすべてのトークンを，1つのデータとしてまとめたもの
*/

class CTokenManager
{
	// メンバ関数
	public:
		// 文字列からトークン列生成
		std::vector<std::string> split_string(const std::string& str, const std::string& delim) const;

		// トークン列からトークン取得
		std::string get_single_token(const std::vector<std::string>& tokens, const int index) const;
		std::vector<std::string> get_token_range(const std::vector<std::string>& tokens, const size_t begin_pos, const size_t end_pos) const;
		std::string get_catnated_tokens(const std::vector<std::string>& tokens, const size_t begin_pos, const size_t end_pos) const;

	private:
		// トークン列生成補助関数
		void erase_control_letter(std::string& str) const;
		void erase_first_delimiters(std::string& str, const std::string& delim) const;
		void erase_first_token(std::string& str, const std::string& delim) const;
		std::string get_first_token(const std::string& str, const std::string& delim) const;

		// 部分文字列削除
		void erase_substring(std::string& str, const std::string& erase_str) const;

	// メンバ変数
	private:
		// なし
};
