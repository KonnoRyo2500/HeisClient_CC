/**
*	@file		game.h
*	@brief		heis ゲームベースクラス
*	@author		Ryo Konno
*	@details	各種ゲームモードのインターフェイスを定義する。
*/
#pragma once

/**
*	@brief ゲーム基本クラス
*/
class CGame {
	// メンバ関数
	public:
		// コンストラクタ
		CGame();
		// デストラクタ
		virtual ~CGame();

		// 対戦の実行
		virtual void play_game() = 0;
};