/**
*	@file		command_executor_factory.cpp
*	@brief		PS コマンド実行ファクトリークラス
*	@author		Ryo Konno
*	@details	PSの各種コマンドを実行するクラスのインスタンスを生成する。
*/

#include "command_executor_factory.h"
#include "recv_command_executor.h"
#include "send_command_executor.h"

/**
*   @brief コマンド実行クラスのインスタンスを生成する
*	@param[in] command_name コマンド名
*	@param[in] socket_first 先攻プレイヤー用ソケット
*	@param[in] socket_second 後攻プレイヤー用ソケット
*	@return CCommandExecutorBase* コマンド実行クラスのアドレス(未定義のコマンドの場合はNULL)
*/
CCommandExecutorBase* CCommandExecutorFactory::create_instance(
	const std::string& command_name,
	const CSocket& socket_first,
	const CSocket& socket_second
) const
{
	CCommandExecutorBase *instance_addr;

	if (command_name == "recv") {
		CRecvCommandExecutor* recv_executor = new CRecvCommandExecutor();
		recv_executor->set_sockets(socket_first, socket_second);
		instance_addr = (CCommandExecutorBase*)recv_executor;
	}
	else if (command_name == "send") {
		CSendCommandExecutor* send_executor = new CSendCommandExecutor();
		send_executor->set_sockets(socket_first, socket_second);
		instance_addr = (CCommandExecutorBase*)send_executor;
	}
	else {
		instance_addr = NULL;
	}

	return instance_addr;
}
