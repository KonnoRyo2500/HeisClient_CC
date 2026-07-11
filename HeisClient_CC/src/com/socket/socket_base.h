/**
*	@file		socket_base.h
*	@brief		TCP/IPソケットベースクラス
*	@author		Ryo Konno
*	@details	各種プラットフォームにおけるソケットクラスのベースクラス。
*/

#pragma once

#include <string>

/**
*	@brief TCP/IPソケットベースクラス
*/
class CSocketBase
{
	// メンバ変数
public:
	// ユーザコードで呼ばれる関数はCSocketクラスで定義する

	// 標準のソケットAPIと関数名が衝突しないよう、ソケットAPIの関数名とは別の関数名にする
	// ソケットAPIでは名前空間が定義されていない可能性もあるため、名前衝突は関数名の変更で回避する必要がある

	// ソケットに名前を付ける
	virtual void wrap_bind(const uint16_t dst_port, const std::string& src_addr) = 0;
	// 相手からの接続を受け付ける
	virtual void wrap_listen() = 0;
	// 相手からの接続を待つ
	virtual void wrap_accept() = 0;
	// ソケットを接続する
	virtual void wrap_connect(const std::string& addr, const uint16_t port) const = 0;
	// データを送信する
	virtual void wrap_send(const std::string& data, const char terminal) = 0;
	// データを受信する
	virtual std::string wrap_recv(const char terminal) = 0;
	// ソケットを閉じる
	virtual void wrap_close() = 0;
	// ソケットを作成する
	virtual int wrap_socket() = 0;

	// 初期化処理を行う
	virtual bool initialize() = 0;
	// 終了処理を行う
	virtual bool finalize() = 0;

	// メンバ変数
protected:
	//! ソケットの実体
	int m_socket;
};