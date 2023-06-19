/**
*	@file		ai_factory.h
*	@brief		heis AIファクトリークラス
*	@author		Ryo Konno
*	@details	各種AIインスタンスを生成するためのファクトリークラス。
*/

#pragma once

#include "ai_base.h"

#include <map>

/**
*	@brief	AIファクトリークラス
*/
class CAIFactory
{
// メンバ関数
public:
	// AIインスタンスを生成する
	CAIBase* create_instance(CCommander commander, std::string ai_impl_name) const;
};