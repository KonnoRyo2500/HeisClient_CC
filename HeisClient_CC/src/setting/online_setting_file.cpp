/**
*	@file		online_setting_file.cpp
*	@brief		heis �I�����C�����[�h�ݒ�t�@�C���N���X
*	@author		Ryo Konno
*	@details	�I�����C�����[�h�̐ݒ�t�@�C����ǂݏ�������N���X�B
*/

#include "online_setting_file.h"
#include "setting_keys.h"

/**
*	@brief �ݒ�l���I�����C�����[�h�ݒ�t�@�C������ǂݍ���
*	@param[in] path �I�����C�����[�h�ݒ�t�@�C���̃p�X
*	@return OnlineSetting �ǂݍ��܂ꂽ�ݒ�l
*/
OnlineSetting COnlineSettingFile::load(const std::string& path) const
{
	// CSV�̓ǂݍ���
	CsvRecords records = parse_csv(path);

	// �ݒ�l��ݒ�l�\���̂Ɋi�[����
	OnlineSetting setting;

	setting.server_ip_addr = records[ONLINE_SETTING_KEY_SVR_ADDR][0];
	setting.server_port_num = atoi(records[ONLINE_SETTING_KEY_SVR_PORT][0].c_str());
	setting.team_name = records[ONLINE_SETTING_KEY_TEAM_NAME][0];
	setting.ai_impl = records[ONLINE_SETTING_KEY_AI_IMPL][0];

	return setting;
}

/**
*	@brief �ݒ�l���I�����C�����[�h�ݒ�t�@�C���ɏ�������
*	@param[in] path �I�����C�����[�h�ݒ�t�@�C���̃p�X
*	@param[in] setting �������ސݒ�l
*/
void COnlineSettingFile::save(const std::string& path, const OnlineSetting& setting) const
{
	// �����_�ŃI�����C�����[�h�ݒ�t�@�C���ɐݒ�l��ۑ�����K�v���Ȃ����߁ADo Nothing
	// �ۑ�����K�v����������A�K�X��������
}