/**
*	@file		local_setting_file.h
*	@brief		heis ���[�J�����[�h�ݒ�t�@�C���N���X
*	@author		Ryo Konno
*	@details	���[�J�����[�h�̐ݒ�t�@�C����ǂݏ�������N���X�B
*/

#pragma once

#include "setting_file_base.h"
#include "common_structure.h"
#include "setting.h"

/**
*	@brief	���[�J�����[�h�ݒ�t�@�C���N���X
*/
class CLocalSettingFile : public CSettingFileBase<LocalSetting>
{
	// �����o�֐�
public:
	// �ݒ�l�����[�J�����[�h�ݒ�t�@�C������ǂݍ���
	LocalSetting load(std::string path) const;
	// �ݒ�l�����[�J�����[�h�ݒ�t�@�C���ɏ�������
	void save(std::string path, LocalSetting setting) const;

private:
	// �e���m�̏����ʒu�̏����A�Ֆʏ�̈ʒu��\���\���̂ɕϊ�����
	std::vector<BoardPosition> build_init_positions(
		std::vector<std::string> init_x_from_csv,
		std::vector<std::string> init_y_from_csv
	) const;
};