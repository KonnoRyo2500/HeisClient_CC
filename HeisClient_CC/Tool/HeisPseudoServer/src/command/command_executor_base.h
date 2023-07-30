/**
*	@file		command_executor_base.h
*	@brief		PS コマンド実行ベースクラス
*	@author		Ryo Konno
*	@details	PSの各種コマンドを実行するクラスの基本クラス。
*/

#pragma once

#include <string>
#include <vector>

/**
*	@brief コマンド実行ベースクラス
*/
class CCommandExecutorBase
{
	// メンバ関数
public:
	// コマンドを実行する
	virtual void execute(const std::vector<std::string>& command) = 0;
};
