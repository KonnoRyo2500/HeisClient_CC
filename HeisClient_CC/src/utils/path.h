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
std::string join(const std::vector<std::string>& paths);
// ���s�t�@�C���̃f�B���N�g���𓾂�
std::string build_exe_dir();

/* �f�B���N�g���� */
#ifdef WIN32
/**
*	@def CC_ROOT_DIR
*	@brief CC�̃��[�g�f�B���N�g��
*/
#define CC_ROOT_DIR join({build_exe_dir(), "..", "..", "..", "..", ".."})

/**
*	@def CC_PROJECT_DIR
*	@brief CC�̃v���W�F�N�g�f�B���N�g��
*/
#define CC_PROJECT_DIR join({CC_ROOT_DIR, "project_win"})
#else
/**
*	@def CC_ROOT_DIR
*	@brief CC�̃��[�g�f�B���N�g��
*/
#define CC_ROOT_DIR join({build_exe_dir(), "..", "..", ".."})

/**
*	@def CC_PROJECT_DIR
*	@brief CC�̃v���W�F�N�g�f�B���N�g��
*/
#define CC_PROJECT_DIR join({CC_ROOT_DIR, "project_linux"})
#endif // WIN32

/**
*	@def CC_SETTING_DIR
*	@brief �ݒ�t�@�C���̃f�B���N�g����
*/
#define CC_SETTING_DIR join({CC_ROOT_DIR, "setting", "CC"})

