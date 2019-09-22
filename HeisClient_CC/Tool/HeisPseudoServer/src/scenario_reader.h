// heisクライアント用JSON送受信治具 シナリオ読み込みクラス
// Author: Ryo Konno

#include <string>
#include <vector>
#include <fstream>

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
			// 全アクション実行完了: シナリオファイルに記載されたアクションをすべて実行した
			ActionType_AllActionDone,
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
		std::string get_message_to_send();
		std::string get_filename_to_send();

	private:
		// トークン作成
		std::vector<std::string> split_action_message(const std::string& action_msg);
		void delete_front_space(std::string& src_str);
		std::string cut_front_token(std::string& src_str);

		// アクション解析
		

	// メンバ変数
	private:
		// 次のアクション
		ActionType m_next_action_type;
		// シナリオファイル
		std::ifstream m_scenario_file;
};
