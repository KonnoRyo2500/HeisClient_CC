/**
*	@file		string_util.cpp
*	@brief		heis �����񏈗��N���X
*	@author		Ryo Konno
*	@details	������Ɋւ���e�폈�����s�����[�e�B���e�B�N���X�B
*/

#include "string_utils.h"

/**
*	@brief ����������̕����ŋ�؂�
*	@param[in] src ������
*	@param[in] delimiter ��؂蕶��
*	@return std::vector<std::string> ������̕�����
*/
std::vector<std::string> CStringUtil::split(const std::string& src, const char delimiter)
{
	std::vector<std::string> tokens;

	int prev_delimiter_idx = -1;
	for (int i = 0; i < src.size(); i++) {
		if (src[i] != delimiter && i < src.size() - 1) {
			continue;
		}

		// ��؂蕶��(or src�̖���)�ɓ��B������A�O�̋�؂蕶��(or src�̐擪)���猻�݂̋�؂蕶���Ԃɂ��镶�����tokens�ɒǉ�����
		int token_last_char_idx = (src[i] != delimiter && i == src.size() - 1 ? i : i - 1);
		int token_len = token_last_char_idx - prev_delimiter_idx;

		if (token_len == 0) {
			prev_delimiter_idx = i;
			continue;
		}
		
		std::string token = src.substr(prev_delimiter_idx + 1, token_len);
		tokens.push_back(token);

		prev_delimiter_idx = i;
	}

	return tokens;
}