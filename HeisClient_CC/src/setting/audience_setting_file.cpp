/**
*	@file		audience_setting_file.cpp
*	@brief		heis �ϐ탂�[�h�ݒ�t�@�C���N���X
*	@author		Ryo Konno
*	@details	�ϐ탂�[�h�ݒ�t�@�C����ǂݏ�������N���X�B
*/

#include "audience_setting_file.h"
#include "setting_keys.h"

/**
*	@brief �ݒ�l���ϐ탂�[�h�ݒ�t�@�C������ǂݍ���
*	@param[in] path �ϐ탂�[�h�ݒ�t�@�C���̃p�X
*	@return OnlineSetting �ǂݍ��܂ꂽ�ݒ�l
*/
AudienceSetting CAudienceSettingFile::load(const std::string& path) const
{
	// CSV�̓ǂݍ���
	CsvRecords records = parse_csv(path);

	// �ݒ�l��ݒ�l�\���̂Ɋi�[����
	AudienceSetting setting;

	setting.server_ip_addr = records[AUDIENCE_SETTING_KEY_SVR_ADDR][0];
	setting.server_port_num = atoi(records[AUDIENCE_SETTING_KEY_SVR_PORT][0].c_str());

	return setting;
}

/**
*	@brief �ݒ�l���ϐ탂�[�h�ݒ�t�@�C���ɏ�������
*	@param[in] path �ϐ탂�[�h�ݒ�t�@�C���̃p�X
*	@param[in] setting �������ސݒ�l
*/
void CAudienceSettingFile::save(const std::string& path, const AudienceSetting& setting) const
{
	// �����_�Ŋϐ탂�[�h�ݒ�t�@�C���ɐݒ�l��ۑ�����K�v���Ȃ����߁ADo Nothing
	// �ۑ�����K�v����������A�K�X��������
}