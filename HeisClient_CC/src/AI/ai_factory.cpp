/**
*	@file		ai_factory.cpp
*	@brief		heis AI�t�@�N�g���[�N���X
*	@author		Ryo Konno
*	@details	�e��AI�C���X�^���X�𐶐����邽�߂̃t�@�N�g���[�N���X�B
*/

#include "ai_factory.h"
#include "sample_ai.h"

/**
*	@brief AI�C���X�^���X�𐶐�����
*	@param[in] commander �i�ߊ��C���X�^���X
*	@param[in] ai_impl_name AI�����̖��O
*	@return CAIBase* AI�C���X�^���X�̃A�h���X
*/
CAIBase* CAIFactory::create_instance(const CCommander& commander, const std::string& ai_impl_name) const
{
	CAIBase *ai_instance_addr;

	// AI�C���X�^���X���쐬
	if (ai_impl_name == "Sample") {
		ai_instance_addr = new CSampleAI(commander);
	}
	else {
		ai_instance_addr = NULL;
	}

	return ai_instance_addr;
}