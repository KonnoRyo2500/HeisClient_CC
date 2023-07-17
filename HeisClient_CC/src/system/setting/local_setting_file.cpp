/**
*	@file		local_setting_file.cpp
*	@brief		heis ���[�J�����[�h�ݒ�t�@�C���N���X
*	@author		Ryo Konno
*	@details	���[�J�����[�h�̐ݒ�t�@�C����ǂݏ�������N���X�B
*/

#include "local_setting_file.h"
#include "setting_keys.h"

/**
*	@brief �ݒ�l�����[�J�����[�h�ݒ�t�@�C������ǂݍ���
*	@param[in] path ���[�J�����[�h�ݒ�t�@�C���̃p�X
*	@return LocalSetting �ǂݍ��܂ꂽ�ݒ�l
*/
LocalSetting CLocalSettingFile::load(const std::string& path) const
{
	// CSV�̓ǂݍ���
	CsvRecords records = parse_csv(path);

	// �ݒ�l��ݒ�l�\���̂Ɋi�[����
	LocalSetting setting;

	setting.my_team_name = records[LOCAL_SETTING_KEY_MY_NAME][0];
	setting.enemy_team_name = records[LOCAL_SETTING_KEY_ENEMY_NAME][0];
	setting.my_team_ai_impl = records[LOCAL_SETTING_KEY_MY_AI_IMPL][0];
	setting.enemy_team_ai_impl = records[LOCAL_SETTING_KEY_ENEMY_AI_IMPL][0];
	setting.board_width = atoi(records[LOCAL_SETTING_KEY_BOARD_WIDTH][0].c_str());
	setting.board_height = atoi(records[LOCAL_SETTING_KEY_BOARD_HEIGHT][0].c_str());
	setting.my_team_init_pos = build_init_positions(
		records[LOCAL_SETTING_KEY_MY_INIT_COORD_X],
		records[LOCAL_SETTING_KEY_MY_INIT_COORD_Y]
	);
	setting.enemy_team_init_pos = build_init_positions(
		records[LOCAL_SETTING_KEY_ENEMY_INIT_COORD_X],
		records[LOCAL_SETTING_KEY_ENEMY_INIT_COORD_Y]
	);
	setting.first_turn_team = records[LOCAL_SETTING_KEY_FIRST_TEAM][0];

	return setting;
}

/**
*	@brief �ݒ�l�����[�J�����[�h�ݒ�t�@�C���ɏ�������
*	@param[in] path ���[�J�����[�h�ݒ�t�@�C���̃p�X
*	@param[in] setting �������ސݒ�l
*/
void CLocalSettingFile::save(const std::string& path, const LocalSetting& setting) const
{
	// �����_�Ń��[�J�����[�h�ݒ�t�@�C���ɐݒ�l��ۑ�����K�v���Ȃ����߁ADo Nothing
	// �ۑ�����K�v����������A�K�X��������
}

/**
*	@brief �e���m�̏����ʒu�̏����A�Ֆʏ�̈ʒu��\���\���̂ɕϊ�����
*	@param[in] init_x_from_csv �e���m��x���W
*	@param[in] init_y_from_csv �e���m��y���W
*	@return std::vector<Coordinate2D> �e���m�̏����ʒu
*/
std::vector<Coordinate2D> CLocalSettingFile::build_init_positions(
	const std::vector<std::string>& init_x_from_csv,
	const std::vector<std::string>& init_y_from_csv
) const
{
	// x���W�Ey���W�̐��������łȂ���΃G���[
	if (init_x_from_csv.size() != init_y_from_csv.size()) {
		throw std::runtime_error("���m�̏����z�u���W��x���W�Ey���W�͂��ꂼ�ꓯ���������ݒ肷��K�v������܂�");
	}

	// �w�肳�ꂽx���W�Ey���W��Ֆʏ�̈ʒu�ɕϊ�����
	std::vector<Coordinate2D> init_positions;
	for (int i = 0; i < init_x_from_csv.size(); i++) {
		Coordinate2D pos = Coordinate2D(
			atoi(init_x_from_csv[i].c_str()),
			atoi(init_y_from_csv[i].c_str())
		);
		
		init_positions.push_back(pos);
	}

	return init_positions;
}