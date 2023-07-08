/**
*	@file		ai_base.cpp
*	@brief		heis AIベースクラス
*	@author		Ryo Konno
*	@details	heis対戦AIのベースクラス。
*/

#include "ai_base.h"

/**
*	@brief コンストラクタ
*	@param[in] commander 司令官インスタンス
*/
CAIBase::CAIBase(const CCommander& commander)
	: m_commander(commander)
{
	// Do Nothing
}