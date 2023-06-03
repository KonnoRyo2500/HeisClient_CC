/**
*	@file		path.h
*	@brief		heis �p�X����
*	@author		Ryo Konno
*	@details	�p�X����̂��߂̊e��֐����`����B
*/

#pragma once

#include <string>
#include <vector>

// �p�X���������������
std::string join(std::vector<std::string> paths);
// CC�̃��[�g�f�B���N�g���𓾂�
std::string build_cc_root_dir();

/* �f�B���N�g���� */
/**
*	@def �ݒ�t�@�C���̃f�B���N�g����
*/
#define CC_SETTING_DIR join({build_cc_root_dir(), "setting", "CC"})

#ifdef WIN32
/**
*	@def CC�̃v���W�F�N�g�f�B���N�g��
*/
#define CC_PROJECT_DIR join({build_cc_root_dir(), "project_win"})
#else
/**
*	@def CC�̃v���W�F�N�g�f�B���N�g��
*/
#define CC_PROJECT_DIR join({build_cc_root_dir(), "project_linux"})
#endif // WIN32