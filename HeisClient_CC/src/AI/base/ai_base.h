/**
*	@file		ai_base.h
*	@brief		heis AIベースクラス
*	@author		Ryo Konno
*	@details	heis対戦AIのベースクラス。
*/

#pragma once

#include "commander.h"

/**
*	@brief	AIベースクラス
*	@detail	heis対戦AIのベースクラス。ユーザは、このクラスを継承してAIを実装する
*	@remark このクラスはインスタンス化できない(抽象クラス)
*/
class CAIBase
{
// メンバ関数
public:
	// コンストラクタ
	explicit CAIBase(const CCommander& commander);

	// メイン処理
	virtual void AI_main(const JSONRecvPacket_Board& board_pkt) = 0;

// メンバ変数
protected:
	//! 兵士を操作するための司令官
	CCommander m_commander;
};