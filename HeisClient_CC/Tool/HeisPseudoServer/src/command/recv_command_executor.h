/**
*	@file		recv_command_executor.h
*	@brief		PS recvコマンド実行クラス
*	@author		Ryo Konno
*	@details	PSのrecvコマンドを実行する。
*/

#pragma once

#include "command_executor_base.h"
#include "socket.h"

/**
*	@brief recvコマンド実行クラス
*/
class CRecvCommandExecutor : public CCommandExecutorBase
{
	// メンバ関数
public:
	// コマンドを実行する
	void execute(const std::vector<std::string>& command);

	// ソケットを登録する
	void set_sockets(const CSocket& socket_first, const CSocket& socket_second);

private:
	// JSONを受信する
	std::string recv_json(const std::string& from);

	// オプションに応じて、JSONを出力する
	void output_json(const std::vector<std::string>& command, const std::string& json);
	// JSONをファイルに出力する
	void output_json_to_file(const std::string& filename, const std::string& json);
	// JSONを標準出力に出力する
	void output_json_to_stdout(const std::string& json);

	// メンバ変数
private:
	//! 先攻プレイヤー用ソケット
	CSocket m_socket_first;
	//! 後攻プレイヤー用ソケット
	CSocket m_socket_second;
};
