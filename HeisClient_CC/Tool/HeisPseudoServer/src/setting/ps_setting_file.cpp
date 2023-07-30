/**
*	@file		ps_setting_file.cpp
*	@brief		heis PS�ݒ�t�@�C���N���X
*	@author		Ryo Konno
*	@details	PS�̐ݒ�t�@�C����ǂݏ�������N���X�B
*/

#include "ps_setting_file.h"
#include "ps_setting_keys.h"

/**
*	@brief �ݒ�l��PS�ݒ�t�@�C������ǂݍ���
*	@param[in] path PS�ݒ�t�@�C���̃p�X
*	@return PsSetting �ǂݍ��܂ꂽ�ݒ�l
*/
PsSetting CPsSettingFile::load(const std::string& path) const
{
	// CSV�̓ǂݍ���
	CsvRecords records = parse_csv(path);

	// �ݒ�l��ݒ�l�\���̂Ɋi�[����
	PsSetting setting;

	setting.scenario_file_name = records[PS_SETTING_KEY_SCENARIO_NAME][0];
	setting.listen_port_num_first = atoi(records[PS_SETTING_KEY_LISTEN_PORT_FIRST][0].c_str());
	setting.listen_port_num_second = atoi(records[PS_SETTING_KEY_LISTEN_PORT_SECOND][0].c_str());
	setting.listen_addr_first = records[PS_SETTING_KEY_LISTEN_ADDR_FIRST][0];
	setting.listen_addr_second = records[PS_SETTING_KEY_LISTEN_ADDR_SECOND][0];

	return setting;
}

/**
*	@brief �ݒ�l��PS�ݒ�t�@�C���ɏ�������
*	@param[in] path PS�ݒ�t�@�C���̃p�X
*	@param[in] setting �������ސݒ�l
*/
void CPsSettingFile::save(const std::string& path, const PsSetting& setting) const
{
	// �����_��PS�ݒ�t�@�C���ɐݒ�l��ۑ�����K�v���Ȃ����߁ADo Nothing
	// �ۑ�����K�v����������A�K�X��������
}