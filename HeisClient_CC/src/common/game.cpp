/**
*	@file		game.cpp
*	@brief		heis ゲーム基本クラス
*	@author		Ryo Konno
*	@details	各種ゲームモードの基本となる操作を定義する．各ゲームモードクラスは，このクラスを継承して作成する．
*/

#include "game.h"

/* public関数 */

/* private関数 */

/**
*	@brief 勝敗を判定する関数
*	@remark 勝敗判定が必要ない場合(観戦モードなど)を考慮し，純粋仮想関数にはしない
*/
bool CGame::judge_win()
{
	return true;
}