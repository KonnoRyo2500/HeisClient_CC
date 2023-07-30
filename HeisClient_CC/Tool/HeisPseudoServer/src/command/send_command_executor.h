/**
*	@file		send_command_executor.h
*	@brief		PS sendコマンド実行クラス
*	@author		Ryo Konno
*	@details	PSのsendコマンドを実行する。
*/

#pragma once

#include "command_executor_base.h"
#include "socket.h"

/**
*	@brief sendコマンド実行クラス
*/
class CSendCommandExecutor : public CCommandExecutorBase
{
	// メンバ関数
public:
	// コマンドを実行する
	void execute(const std::vector<std::string>& command);

	// ソケットを登録する
	void set_sockets(const CSocket& socket_first, const CSocket& socket_second);

private:
	// オプションに応じて、JSONを読み込む
	std::string load_json(const std::vector<std::string>& command);
	// ファイルからJSONを読み込む
	std::string load_json_from_file(const std::string& path);

	// JSONを送信する
	void send_json(const std::string& to, const std::string& json);

	// メンバ変数
private:
	//! 先攻プレイヤー用ソケット
	CSocket m_socket_first;
	//! 後攻プレイヤー用ソケット
	CSocket m_socket_second;
};
