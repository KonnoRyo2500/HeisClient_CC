/**
*	@file		game.h
*	@brief		heis ゲーム基本クラス
*	@author		Ryo Konno
*	@details	各種ゲームモードの基本となる操作を定義する．各ゲームモードクラスは，このクラスを継承して作成する．
*/
#pragma once

/**
*	@brief ゲーム基本クラス
*/
class CGame {
	// 構造体，列挙体など
	private:

	// メンバ関数
	public:
		// コンストラクタ
		CGame();
		// デストラクタ
		virtual ~CGame();

		// 対戦の実行
		/**
		*	@brief 対戦メイン処理
		*/
		virtual void play_game() = 0;

	private:
		// 勝敗判定
		virtual bool judge_win();

	// メンバ変数
	private:
		// なし
};