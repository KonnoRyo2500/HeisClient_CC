// heis ユーザAIクラス
// Author: Ryo Konno
#pragma once

#include "commander.h"

// 対戦用のAI(ユーザAI)は，このクラスに記述すること(必要があれば，メンバ関数やメンバ変数，クラスを新たに追加してもよい)
class CUserAI {
	// 構造体，列挙体など

	// メンバ関数
	public:
		// コンストラクタ
		explicit CUserAI(CCommander* commander);

		// メイン処理
		void AI_main();

	// メンバ変数
	private:
		CCommander* m_commander;
};
