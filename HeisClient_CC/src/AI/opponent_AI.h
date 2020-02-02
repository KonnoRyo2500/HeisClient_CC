// heis ローカルモード用対戦相手AIクラス
// Author: Ryo Konno
#pragma once

#include "commander.h"

// ローカルモードで対戦相手として使用するAIは，このクラスに記述すること(必要があれば，メンバ関数やメンバ変数，クラスを新たに追加してもよい)
class COpponentAI {
		// 構造体，列挙体など
	private:
		// 行動の種類(サンプルAI用)
		enum SampleAction {
			SampleAction_Move,
			SampleAction_Attack,
		};

		// メンバ関数
	public:
		// コンストラクタ
		explicit COpponentAI(CCommander* commander);

		// メイン処理
		void AI_main(const JSONRecvPacket_Field& field_pkt);

	private:
		// サンプルAI用の処理
		void sample_random_move(const std::string infantry_id);
		void sample_random_attack(const std::string infantry_id);
		std::string sample_select_next_infantry(const std::vector<std::string>& infantry_ids) const;
		SampleAction sample_decide_action() const;

		// メンバ変数
	private:
		// 敵チームの兵士を操作するための司令官
		CCommander* m_commander;
};