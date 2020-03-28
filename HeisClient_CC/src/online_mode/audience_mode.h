/**
*	@file		audience_mode.h
*	@brief		heis 観戦モードクラス
*	@author		Ryo Konno
*	@details	heisの対戦を観戦する．
*/
#pragma once

#include "client_socket.h"
#include "log.h"

extern CLog g_system_log;

/**
*	@brief	観戦モードクラス
*/
class CAudienceMode {
	// 構造体，列挙体など
	private:

	// メンバ関数
	public:
		void watch_game();

	private:
		// 観戦の初期化
		void initialize_watch();

		// 観戦の終了処理
		void finalize_watch();

	// メンバ変数
	private:
		//! TCPソケット
		CClientSocket *m_sck;
};