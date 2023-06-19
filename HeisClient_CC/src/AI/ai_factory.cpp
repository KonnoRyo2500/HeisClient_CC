/**
*	@file		ai_factory.cpp
*	@brief		heis AIファクトリークラス
*	@author		Ryo Konno
*	@details	各種AIインスタンスを生成するためのファクトリークラス。
*/

#include "ai_factory.h"
#include "sample_ai.h"

/**
*	@brief AIインスタンスを生成する
*	@param[in] commander 司令官インスタンス
*	@param[in] ai_impl_name AI実装の名前
*	@return CAIBase* AIインスタンスのアドレス
*/
CAIBase* CAIFactory::create_instance(const CCommander& commander, const std::string& ai_impl_name) const
{
	CAIBase *ai_instance_addr;

	// AIインスタンスを作成
	if (ai_impl_name == "Sample") {
		ai_instance_addr = new CSampleAI(commander);
	}
	else {
		ai_instance_addr = NULL;
	}

	return ai_instance_addr;
}