/**
*	@file		socket_base.h
*	@brief		TCP/IP通信用ソケット基本クラス
*	@author		Ryo Konno
*	@details	各種TCP/IPソケットクラスの基本となるクラス。
*/

#pragma once

#include <string>

/**
*	@brief ソケット基本クラス
*	@remark 本クラスのインスタンスを作成することは非推奨。派生クラスを利用すること
*/
class CSocketBase {
	// 構造体・列挙体など
	protected:
		/**
		*	@enum SocketConstVal
		*	ソケットに関する諸定数
		*/
		enum SocketConstVal {
			//! 受信バッファサイズ
			SocketConstVal_RecvBufSize = 1000,
		};

	// メンバ関数
	public:
		// コンストラクタ
		CSocketBase();
		// デストラクタ
		virtual ~CSocketBase();


		// 送受信
		virtual void sck_send(const std::string& data, const char etx = '\n') const;
		virtual std::string sck_recv(const char etx = '\n');

		// 一部の操作(bindやacceptなど)は、派生クラス側で定義する
		// TODO: このクラスにすべての操作を定義してしまってもよいか検討する
	protected:
		// winsockの初期化処理
		// 終了処理はWSACleanupを呼べば済むので定義しない
		virtual void initialize_winsock() const;

		// ソケットのクローズ
		virtual void sck_close() const;

		// ソケットの生成
		virtual void sck_socket();

	// メンバ変数
	protected:
		//! ソケットの実体
		int m_sck;

		//! 前回の受信時に余分に受信したデータ
		std::string m_prev_recv_remaind_data;
};