// heis�N���C�A���g�pJSON����M���� �V�i���I�ǂݍ��݃N���X
// Author: Ryo Konno

#include "scenario_reader.h"
#include "heis_client_exception.h"

/*
	�R���X�g���N�^
	����1: const std::string& scenario_file_name �V�i���I�t�@�C����
	��O: �V�i���I�t�@�C���̃I�[�v���Ɏ��s�����Ƃ�
*/
CScenarioReader::CScenarioReader(const std::string& scenario_file_name)
	: m_next_action_type(ActionType_AllActionDone)
{
	m_scenatio_file = fopen(scenario_file_name.c_str(), "r");
	if(!m_scenatio_file){
		throw CHeisClientException("�V�i���I�t�@�C���̃I�[�v���Ɏ��s���܂���(�t�@�C����: %s)", scenario_file_name.c_str());
	}
}

/*
	�f�X�g���N�^
	�����Ȃ�
*/
CScenarioReader::~CScenarioReader()
{
	fclose(m_scenatio_file);
	m_scenatio_file = NULL;
}


