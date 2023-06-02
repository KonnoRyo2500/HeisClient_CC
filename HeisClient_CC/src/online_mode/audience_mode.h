/**
*	@file		audience_mode.h
*	@brief		heis 観戦モードクラス
*	@author		Ryo Konno
*	@details	heisの対戦を観戦する．
*/
#pragma once

#include "game.h"
#include "client_socket.h"
#include "log.h"
#include "setting.h"

extern CLog *g_system_log;

/**
*	@brief	観戦モードクラス
*	@remark 他のモードと同様にインスタンスを取り扱いたいため，CGameクラスの派生クラスとする
*/
class CAudienceMode : public CGame {
	// 構造体，列挙体など
	private:

	// メンバ関数
	public:
		// 観戦メイン処理
		void play_game() override;

	private:
		// 観戦の初期化
		void initialize_watch(AudienceSetting setting);

		// 観戦の終了処理
		void finalize_watch();

	// メンバ変数
	private:
		//! TCPソケット
		CClientSocket *m_sck;
};