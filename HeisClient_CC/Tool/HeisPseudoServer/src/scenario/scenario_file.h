/**
*	@file		scenario_file.h
*	@brief		PS �V�i���I�t�@�C���N���X
*	@author		Ryo Konno
*	@details	PS�̃V�i���I�t�@�C���Ɋւ���e�푀���񋟂���B
*/

#pragma once

#include <vector>
#include <string>

/**
*	@brief �V�i���I�t�@�C���N���X
*/
class CScenarioFile
{
	// �N���X�֐�
public:
	// �V�i���I�t�@�C�������[�h����
	static std::vector<std::vector<std::string>> load(const std::string& path);
};
