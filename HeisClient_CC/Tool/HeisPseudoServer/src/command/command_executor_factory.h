/**
*	@file		command_executor_factory.h
*	@brief		PS コマンド実行ファクトリークラス
*	@author		Ryo Konno
*	@details	PSの各種コマンドを実行するクラスのインスタンスを生成する。
*/

#pragma once

#include "command_executor_base.h"
#include "socket.h"

/**
*	@brief コマンド実行ファクトリークラス
*/
class CCommandExecutorFactory
{
	// メンバ関数
public:
	// コマンド実行クラスのインスタンスを生成する
	CCommandExecutorBase* create_instance(
		const std::string& command_name,
		const CSocket& socket_first,
	const CSocket& socket_second
	) const;
};
