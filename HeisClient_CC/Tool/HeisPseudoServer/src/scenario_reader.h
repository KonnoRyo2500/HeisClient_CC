// heisクライアント用JSON送受信治具 シナリオ読み込みクラス
// Author: Ryo Konno

#include <string>
#include <vector>
#include <fstream>

#include "token_manager.h"

class CScenarioReader
{
	// 構造体・列挙体など
	public:
		// アクションの種類
		enum ActionType {
			// 受信: クライアントからメッセージを受信する
			ActionType_Receive,
			// メッセージ送信: シナリオファイルに記載されたメッセージをクライアントに送信する
			ActionType_SendMessage,
			// ファイル送信: シナリオファイルで指定されたファイルの内容をクライアントに送信する
			ActionType_SendFileContents,
			// アクションなし: アクションが記載されていない
			ActionType_None,
			// 全アクション実行完了: シナリオファイルに記載されたアクションをすべて実行した
			ActionType_AllActionDone,
			// 不正: 未定義のアクションが記載されている
			ActionType_Invalid = 999,
		};

	// メンバ関数
	public:
		// コンストラクタ
		explicit CScenarioReader(const std::string& scenario_file_name);
		// デストラクタ
		~CScenarioReader();

		// 次のアクションの種類を取得
		ActionType get_next_aciton_type();

		// 送信するメッセージやファイル名を取得
		std::string get_message_to_send() const;
		std::string get_filename_to_send() const;

	private:
		// アクション解析
		bool is_match_command_part(const token_array_t action, const token_array_t command) const;
		bool is_empty_line(const std::string& action_str) const;

	// メンバ変数
	private:
		// シナリオファイル
		std::ifstream m_scenario_file;
		// 直前に読んだアクション
		token_array_t m_latest_action;
};
