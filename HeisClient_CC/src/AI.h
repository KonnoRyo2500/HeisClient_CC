// heis ユーザAIクラス
// Author: Ryo Konno
#pragma once

#include "commander.h"

// 対戦用のAI(ユーザAI)は，このクラスに記述すること(必要があれば，メンバ関数やメンバ変数，クラスを新たに追加してもよい)
class CUserAI {
	// 構造体，列挙体など
	private:
		enum SampleAction {
			SampleAction_Move,
			SampleAction_Attack,
		};

	// メンバ関数
	public:
		// コンストラクタ
		explicit CUserAI(CCommander* commander);

		// メイン処理
		void AI_main();

	private:
		// サンプルAI用の処理
		std::string sample_select_next_infantry(const std::vector<std::string>& infantry_ids) const;
		SampleAction sample_decide_action() const;
		Direction sample_decide_direction() const;

	// メンバ変数
	private:
		CCommander* m_commander;
};
