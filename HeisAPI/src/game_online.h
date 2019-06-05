// heis ゲーム(オンラインモード)進行管理クラス
// Author: Ryo Konno
#pragma once

#include <string>
#include <vector>

class CGameOnline {
	// 構造体，列挙体など
	public:
		/* JSON関連のデータ(JSONの仕様に関しては，資料「兵ズ通信仕様.ods」を参照すること) */
		// サーバに送信するデータ
		// 「名前」JSONのデータ
		struct JSONSendData_Name {
			std::string team_name;		// "team_name"(フィールド)
		};

		// サーバから受け取るデータ
		// 「名前確定」JSONのデータ
		struct JSONRecvData_NameDecided {
			std::string your_name;		// "your_name"(フィールド)
		};

		// 「結果」JSONに含まれる"result"配列の要素
		struct JSONRecvData_ResultElem {
			std::string unit_id;		// "unit_id"(フィールド)
			std::string error;			// "error"(フィールド)
		};

		// 「結果」JSONのデータ
		struct JSONRecvData_Result {
			std::vector<JSONRecvData_ResultElem> result;	// "result"(配列)
		};

	// メンバ関数
	public:
		void play_game();
		
	private:
		// 対戦開始前処理
		void prepare_to_battle();

		// 対戦開始前処理の実行関数
		void recv_name_request();
		void name_entry(const std::string& name);
		void name_register();

		// 対戦の終了判定
		bool is_battle_end();

		// 対戦後の勝敗判定
		bool judge_win();

	// メンバ変数
		std::string m_team_name;
};