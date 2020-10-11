﻿/**
*	@file		opponent_AI.h
*	@brief		heis ローカルモード用対戦相手AIクラス
*	@author		Ryo Konno
*	@details	ローカルモードの対戦相手となるAIの思考ルーチンを定義する．
*/
#pragma once

#include "commander.h"

/**
*	@brief 対戦相手AIクラス
*	@details ローカルモードで対戦相手として使用するAIは，このクラスに記述すること(必要があれば，メンバ関数やメンバ変数，クラスを新たに追加してもよい)[
*	@remark "sample"とついている関数やenumは，サンプルAI用のものなので，削除してもよい．
*/
class COpponentAI {
		// 構造体，列挙体など
	private:
		/**
		*	@enum SampleAction
		*	行動の種類(サンプルAI用)
		*/
		enum SampleAction {
			//! 移動
			SampleAction_Move,
			//! 攻撃
			SampleAction_Attack,
		};

		// メンバ関数
	public:
		// コンストラクタ
		explicit COpponentAI(CCommander* commander);

		// メイン処理
		void AI_main(const JSONRecvPacket_Board& board_pkt);

	private:
		// サンプルAI用の処理
		void sample_random_move(const std::string infantry_id);
		void sample_random_attack(const std::string infantry_id);
		std::string sample_select_next_infantry(const std::vector<std::string>& infantry_ids) const;
		SampleAction sample_decide_action() const;

		// メンバ変数
	private:
		//! 敵チームの兵士を操作するための司令官
		CCommander* m_commander;
};