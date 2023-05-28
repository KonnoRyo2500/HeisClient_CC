/**
*	@file		sample_ai.h
*	@brief		heis サンプルAI
*	@author		Ryo Konno
*	@details	AIのサンプル実装。ランダムに行動を選択する。
*/

#pragma once

#include "ai_base.h"

/**
*	@brief サンプルAIクラス
*	@details サンプル実装のAI。ランダムに行動を選択します。AI実装の際の参考としてお使いください。
*/
class CSampleAI : public CAIBase
{
	// 構造体・列挙体など
private:
	/**
	*	@enum Action
	*	行動の種類
	*/
	enum Action {
		//! 移動
		Action_Move,
		//! 攻撃
		Action_Attack,
	};

	// メンバ関数
public:
	// コンストラクタ
	explicit CSampleAI(CCommander* commander);

	// メイン処理
	void AI_main(const JSONRecvPacket_Board& board_pkt);

private:
	// ランダムな方向に移動する
	void sample_random_move(const std::string infantry_id);
	// ランダムな方向に攻撃する
	void sample_random_attack(const std::string infantry_id);
	// 次に行動する兵士を選択する
	std::string sample_select_next_infantry(const std::vector<std::string>& infantry_ids) const;
	// 兵士がどの行動をするか選択する
	Action sample_decide_action() const;
};