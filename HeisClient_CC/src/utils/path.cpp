/**
*	@file		path.cpp
*	@brief		heis �p�X����
*	@author		Ryo Konno
*	@details	�p�X����̂��߂̊e��֐����`����B
*/

#include "path.h"

#include <climits>
#ifdef WIN32
#include <Windows.h>
#else
#include <unistd.h>
#endif // WIN32
#include <stdarg.h>
#include <filesystem>

/**
*	@brief �p�X���������������
*	@param[in] paths �p�X������
*	@return std::string ������̃p�X������
*/
std::string join(const std::vector<std::string>& paths)
{
	std::filesystem::path joined_path;

	for (std::string p : paths) {
		joined_path.append(p);
	}
	joined_path = joined_path.lexically_normal();

	return joined_path.string();
}


/**
*	@brief ���s�t�@�C���̃f�B���N�g����Ԃ��֐�
*	@return std::string ���s�t�@�C���̃f�B���N�g��
*/
std::string build_cc_root_dir()
{
#ifdef WIN32
	char exe_path_c[MAX_PATH] = { 0 };

	GetModuleFileName(NULL, exe_path_c, sizeof(exe_path_c));
	std::string exe_path_s = std::string(exe_path_c);// <root>\project_win\bin\Debug or Release\x86 or x64\HeisClient_CC.exe
	std::string root_dir = join({exe_path_s, "..", "..", "..", "..", ".."});

	return root_dir;
#else
	char exe_path_c[PATH_MAX] = { 0 };

	if (readlink("/proc/self/exe", exe_path_c, sizeof(exe_path_c) - 1) > 0) {
		std::string exe_path_s = std::string(exe_path_c); // <root>\project_linux\bin\HeisClient_CC
		std::string root_dir = join({exe_path_s, "..", "..", ".."});
		return root_dir;
	}
	else {
		throw std::runtime_error("���s�t�@�C���̃p�X�擾�Ɏ��s���܂���");
	}
#endif
}