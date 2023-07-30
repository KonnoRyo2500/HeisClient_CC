/**
*	@file		ps_setting_file.h
*	@brief		heis PS�ݒ�t�@�C���N���X
*	@author		Ryo Konno
*	@details	PS�̐ݒ�t�@�C����ǂݏ�������N���X�B
*/

#pragma once

#include "setting_file_base.h"

/**
*	@struct PsSetting
*	@brief PS�ݒ�l�i�[�p�\����
*/
struct PsSetting {
	//! �V�i���I�t�@�C����
	std::string scenario_file_name;
	//! ��U����̐ڑ���҂��󂯂�|�[�g�ԍ�
	uint16_t listen_port_num_first;
	//! ��U����̐ڑ���҂��󂯂�|�[�g�ԍ�
	uint16_t listen_port_num_second;
	//! ��U����̐ڑ���҂��󂯂�IP�A�h���X
	std::string listen_addr_first;
	//! ��U����̐ڑ���҂��󂯂�IP�A�h���X
	std::string listen_addr_second;
};

/**
*	@brief	PS�ݒ�t�@�C���N���X
*/
class CPsSettingFile : public CSettingFileBase<PsSetting>
{
	// �����o�֐�
public:
	// �ݒ�l��PS�ݒ�t�@�C������ǂݍ���
	PsSetting load(const std::string& path) const;
	// �ݒ�l��PS�ݒ�t�@�C���ɏ�������
	void save(const std::string& path, const PsSetting& setting) const;
};