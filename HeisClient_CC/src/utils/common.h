/**
*	@file		common.h
*	@brief		heis ���ʏ���
*	@author		Ryo Konno
*	@details	CC���ŋ��ʂ��Ďg�p���鏈�����`����D
*/
#pragma once



#include <string>
#include <vector>

//! ���ʏ����E�f�[�^�\��
namespace cc_common {
	/* �����񏈗� */
	// ������̕���
	std::vector<std::string> split_string(const std::string& str, const std::string& delim);
	// ������̐؂���
	std::string cut_string(std::string& str, const size_t begin_pos, const size_t end_pos = SIZE_MAX);

	/* �e���v���[�g�֐� */
	/**
	*	@brief ����̕����╶���񂪊܂܂�Ă��邩���肷��֐�
	*	@param[in] str ������
	*	@param[in] target �������镶�� or ������
	*	@return bool str��target���܂܂�Ă��邩(�܂܂�Ă����true)
	*/
	template<typename T>
	bool contains(const std::string& str, const T& target)
	{
		return str.find(target) != std::string::npos;
	}

	/**
	*	@brief �t�H�[�}�b�g�������W�J����֐�
	*	@param[in] fmt �t�H�[�}�b�g������
	*	@param[in] args fmt�ɗ^�������
	*	@return std::string fmt��W�J����������
	*/
	template<typename ... Args>
	std::string format(const std::string& fmt, Args ... args)
	{
		// �W�J��̕����񒷂𓾂�(+1��NULL�����̕�)
		size_t expand_len = std::snprintf(NULL, 0, fmt.c_str(), args ...) + 1;
		std::vector<char> expand_str(expand_len);

		std::snprintf(&expand_str[0], expand_len, fmt.c_str(), args ...);
		return std::string(&expand_str[0]);
	}
}