/**
*	@file		setting_file_base.h
*	@brief		heis �ݒ�t�@�C���x�[�X�N���X
*	@author		Ryo Konno
*	@details	CSV�`���̐ݒ�t�@�C����ǂݏ�������e��N���X�̃x�[�X�N���X�B
*/

#pragma once

#include <string>
#include <map>
#include <vector>
#include <fstream>

#include "common.h"

/**
*	@brief �\�������ꂽCSV���R�[�h�̌^
*/
typedef std::map<std::string, std::vector<std::string>> CsvRecords;

/**
*	@brief	�ݒ�t�@�C���x�[�X�N���X
*	@tparam SettingType �ݒ�l�N���X�̌^
*/
template <typename SettingType>
class CSettingFileBase
{
	// �����o�֐�
public:
	// �ݒ�l�����[�J�����[�h�ݒ�t�@�C������ǂݍ���
	virtual SettingType load(const std::string& path) const = 0;
	// �ݒ�l�����[�J�����[�h�ݒ�t�@�C���ɏ�������
	virtual void save(const std::string& path, const SettingType& setting) const = 0;

protected:
	// CSV�t�@�C���̒��g���p�[�X����
	CsvRecords parse_csv(const std::string& path) const;
};

/**
*	@brief CSV�t�@�C��������e�s�̃��R�[�h��ǂݍ��݁A���̒��g���p�[�X����
*	@tparam SettingType �ݒ�t�@�C���N���X�̌^
*	@param[in] path CSV�t�@�C���̃p�X
*	@return CsvRecords �\�������ꂽCSV���R�[�h
*/
template <typename SettingType>
CsvRecords CSettingFileBase<SettingType>::parse_csv(const std::string& path) const
{
	std::ifstream in_stream(path);
	CsvRecords records;

	// �ݒ�t�@�C���̓ǂݍ��݂Ɏ��s����
	if (in_stream.fail()) {
		throw std::runtime_error(cc_common::format("�ݒ�t�@�C��%s�̃I�[�v���Ɏ��s���܂���", path.c_str()));
	}

	// �e�s���p�[�X����
	std::string csv_line;
	while (std::getline(in_stream, csv_line)) {
		std::vector<std::string> parsed_line = cc_common::split_string(csv_line, ",");

		// �L�[���ƍŒ�1�ȏ�̒l��1�s�ɑ��݂��Ȃ���΃G���[
		if (parsed_line.empty()) {
			throw std::runtime_error("�ݒ�t�@�C���ɋ�s������܂�");
		}
		if (parsed_line.size() == 1) {
			throw std::runtime_error("�ݒ�t�@�C���ɒl���������Ă���s������܂�");
		}

		// �p�[�X�����s���瓾��ꂽ�L�[�ƒl���A�\�����ς�CSV���R�[�h(records)�ɒǉ�
		std::string key = parsed_line[0];
		std::vector<std::string> values(parsed_line.size() - 1);
		std::copy(
			parsed_line.begin() + 1,
			parsed_line.end(),
			values.begin()
		);
		records.insert_or_assign(key, values);
	}

	return records;
}