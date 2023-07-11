/**
*	@file		audience_setting_file.h
*	@brief		heis �ϐ탂�[�h�ݒ�t�@�C���N���X
*	@author		Ryo Konno
*	@details	�ϐ탂�[�h�ݒ�t�@�C����ǂݏ�������N���X�B
*/

#pragma once

#include "setting_file_base.h"
#include "common_structure.h"
#include "setting.h"

/**
*	@brief	�ϐ탂�[�h�ݒ�t�@�C���N���X
*/
class CAudienceSettingFile : public CSettingFileBase<AudienceSetting>
{
	// �����o�֐�
public:
	// �ݒ�l���ϐ탂�[�h�ݒ�t�@�C������ǂݍ���
	AudienceSetting load(const std::string& path) const;
	// �ݒ�l���ϐ탂�[�h�ݒ�t�@�C���ɏ�������
	void save(const std::string& path, const AudienceSetting& setting) const;
};