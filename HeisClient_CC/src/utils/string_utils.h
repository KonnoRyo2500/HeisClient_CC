/**
*	@file		string_util.h
*	@brief		heis �����񏈗��N���X
*	@author		Ryo Konno
*	@details	������Ɋւ���e�폈�����s�����[�e�B���e�B�N���X�B
*/

#pragma once

#include <vector>
#include <string>

/**
*	@brief �����񏈗��N���X
*/
class CStringUtil
{
	// �����o�֐�
public:
	// ����������̕����ŋ�؂�
	static std::vector<std::string> split(const std::string& src, const char delimiter);
	// �t�H�[�}�b�g�������W�J����
	template<typename ... Args>
	static std::string format(const std::string& fmt, Args ... args);
};

/**
	*	@brief �t�H�[�}�b�g�������W�J����
	*	@param[in] fmt �t�H�[�}�b�g������
	*	@param[in] args fmt�ɗ^�������
	*	@return std::string fmt��W�J����������
	*/
template<typename ... Args>
std::string CStringUtil::format(const std::string& fmt, Args ... args)
{
	// �W�J��̕����񒷂𓾂�(+1��NULL�����̕�)
	size_t expand_len = std::snprintf(NULL, 0, fmt.c_str(), args ...) + 1;
	std::vector<char> expand_str(expand_len);

	std::snprintf(&expand_str[0], expand_len, fmt.c_str(), args ...);
	return std::string(&expand_str[0]);
}