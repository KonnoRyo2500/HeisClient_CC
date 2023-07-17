/**
*	@file		online_setting_file.h
*	@brief		heis �I�����C�����[�h�ݒ�t�@�C���N���X
*	@author		Ryo Konno
*	@details	�I�����C�����[�h�̐ݒ�t�@�C����ǂݏ�������N���X�B
*/

#pragma once

#include "setting_file_base.h"
#include "setting.h"

/**
*	@brief	�I�����C�����[�h�ݒ�t�@�C���N���X
*/
class COnlineSettingFile : public CSettingFileBase<OnlineSetting>
{
	// �����o�֐�
public:
	// �ݒ�l���I�����C�����[�h�ݒ�t�@�C������ǂݍ���
	OnlineSetting load(const std::string& path) const;
	// �ݒ�l���I�����C�����[�h�ݒ�t�@�C���ɏ�������
	void save(const std::string& path, const OnlineSetting& setting) const;
};