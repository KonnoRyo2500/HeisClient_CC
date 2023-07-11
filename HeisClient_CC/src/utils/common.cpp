/**
*	@file		common.cpp
*	@brief		heis ���ʏ���
*	@author		Ryo Konno
*	@details	CC���ŋ��ʂ��Ďg�p���鏈�����`����D
*/

#include <climits>
#ifdef WIN32
#include <Windows.h>
#else
#include <unistd.h>
#endif // WIN32
#include <stdexcept>

#include "common.h"

/* static�֐� */
//! ���䕶���̍폜
static void erase_control_letter(std::string& str);
//! �擪��؂蕶���̍폜
static void erase_first_delimiters(std::string& str, const std::string& delim);
//! �擪�g�[�N���̍폜
static void erase_first_token(std::string& str, const std::string& delim);
//! �擪�g�[�N���̎擾
static std::string get_first_token(const std::string& str, const std::string& delim);
//! ����������폜
static void erase_substring(std::string& str, const std::string& erase_str);

/**
*	@brief ��������g�[�N����ɕ�������֐�
*	@param[in] str �������̕�����
*	@param[in] delim ��؂蕶��
*	@return std::vector<std::string> �g�[�N����
*/
std::vector<std::string> cc_common::split_string(const std::string& str, const std::string& delim)
{
	// ���̕�����͎c���Ă����������߁C������̕����������ō���Ă���
	std::string str_work(str);
	std::vector<std::string> tokens;

	erase_control_letter(str_work);
	while (str_work.size() > 0) {
		std::string token;

		// ��؂蕶�����폜����
		erase_first_delimiters(str_work, delim);

		// �g�[�N����1�؂�o��
		token = get_first_token(str_work, delim);
		erase_first_token(str_work, delim);

		// �g�[�N����ɒǉ�
		if (token.size() > 0) {
			tokens.push_back(token);
		}
	}
	return tokens;
}

/**
*	@brief ����͈̔͂̕������؂����ĕԂ��֐�
*	@param[in] str ������
*	@param[in] begin_pos �؂���͈͂̎n�[�ʒu(0�n�܂�)
*	@param[in] end_pos �؂���͈͂̏I�[�ʒu(0�n�܂�, �ȗ��B�ȗ�������Astr�̏I�[�������w�肵���ꍇ��str�̏I�[)
*/
std::string cc_common::cut_string(std::string& str, const size_t begin_pos, const size_t end_pos)
{
	// �����̃`�F�b�N
	if (begin_pos >= str.size()) {
		throw std::runtime_error(cc_common::format("�؂���̊J�n�ʒu�������񒆂ɂ���܂���(������̒���: %zu, �J�n�ʒu: %zu)", str.size(), begin_pos));
	}
	if (begin_pos > end_pos) {
		throw std::runtime_error(cc_common::format("�J�n�ʒu���I�[�ʒu�������ɂ���܂�(�J�n�ʒu: %zu, �I�[�ʒu: %zu)", begin_pos, end_pos));
	}

	// ������̐؂���
	std::string ret;
	if (end_pos >= str.size()) {
		ret = str.substr(begin_pos);
		str = str.substr(0, begin_pos);
	}
	else {
		ret = str.substr(begin_pos, end_pos - begin_pos + 1);
		str = str.substr(0, begin_pos) + str.substr(end_pos + 1);
	}

	return ret;
}

/* ����J�֐� */
/**
*	@brief �����񒆂̐��䕶�����폜����֐�
*	@param[out] str ���䕶�����폜����Ώۂ̕�����
*/
static void erase_control_letter(std::string& str)
{
	// �폜���鐧�䕶���ꗗ
	const std::vector<std::string> ctl_code_list = {
		// BOM(UTF-8)
		/*
		  ���̏����ɂ��CSJIS�Łu*���v(*��2�o�C�g�ڂ�0xEF�̕���)���܂ޕ�����͕s���ȕ�����ɂȂ邪�C
		  ���̂悤�ȕ�������܂ރA�N�V�����̓��[�U���^�p�ɂ��V�i���I�t�@�C���ɋL�q���Ȃ����̂Ƃ���D
		*/
		"\xEF\xBB\xBF",
		// CR
		"\r",
		// LF
		"\n",
	};

	for (auto& ctl_code : ctl_code_list) {
		erase_substring(str, ctl_code);
	}
}

/**
*	@brief ������̐擪�ɂ����؂蕶������폜����֐�
*	@param[out] str ��؂蕶������폜����Ώۂ̕�����
*	@param[in] delim ��؂蕶��
*/
static void erase_first_delimiters(std::string& str, const std::string& delim)
{
	// ���ɏo������g�[�N���̐擪�ʒu
	size_t next_token_pos = str.find_first_not_of(delim);

	if (next_token_pos == std::string::npos) {
		// ��؂蕶����̌��Ƀg�[�N�����Ȃ���΁C�����I��
		str.clear();
		return;
	}
	str = str.substr(next_token_pos);
}

/**
*	@brief ������̐擪�ɂ���g�[�N�����폜����֐�
*	@param[out] str �g�[�N�����폜����Ώۂ̕�����
*	@param[in] delim ��؂蕶��
*/
static void erase_first_token(std::string& str, const std::string& delim)
{
	// ���ɏo�������؂蕶����̐擪�ʒu
	size_t next_delims_pos = str.find_first_of(delim);

	if (next_delims_pos == std::string::npos) {
		// �g�[�N���̌��ɋ�؂蕶���񂪂Ȃ���΁C���̃g�[�N�����폜�������I��
		str.clear();
		return;
	}
	str = str.substr(next_delims_pos);
}

/**
*	@brief ������̐擪�ɂ���g�[�N�����擾����֐�
*	@param[out] str �g�[�N�����폜����Ώۂ̕�����
*	@param[in] delim ��؂蕶��
*	@return std::string �擾�����g�[�N��
*/
static std::string get_first_token(const std::string& str, const std::string& delim)
{
	// ���ɏo�������؂蕶����̐擪�ʒu
	size_t next_delims_pos = str.find_first_of(delim);

	if (next_delims_pos == std::string::npos) {
		// �g�[�N���̌��ɋ�؂蕶���񂪂Ȃ���΁C������S�̂��g�[�N���ɂȂ�
		return str;
	}
	return str.substr(0, next_delims_pos);
}

/**
*	@brief �w�肳�ꂽ��������C�����񒆂���폜����֐�
*	@param[out] str ���H�Ώۂ̕�����
*	@param[in] erase_str �폜���镶����
*/
static void erase_substring(std::string& str, const std::string& erase_str)
{
	// TODO: replace���g����΁C���̊֐��͕s�v�ɂȂ邩��?
	size_t substr_pos = str.find(erase_str);
	while (substr_pos != std::string::npos) {
		str.erase(substr_pos, erase_str.size());
		substr_pos = str.find(erase_str);
	}
}