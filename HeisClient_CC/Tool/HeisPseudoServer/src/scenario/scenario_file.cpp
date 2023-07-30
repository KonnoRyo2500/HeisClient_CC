/**
*	@file		scenario_file.h
*	@brief		PS �V�i���I�t�@�C���N���X
*	@author		Ryo Konno
*	@details	PS�̃V�i���I�t�@�C���Ɋւ���e�푀���񋟂���B
*/

#include "scenario_file.h"
#include "string_utils.h"

#include <fstream>
#include <stdexcept>

/**
*	@brief �V�i���I�t�@�C�������[�h����
*	@param[in] path �V�i���I�t�@�C���̃p�X
*	@return std::vector<std::vector<std::string>> �V�i���I�t�@�C������ǂݍ��񂾃R�}���h
*/
std::vector<std::vector<std::string>> CScenarioFile::load(const std::string& path)
{
	// �V�i���I�t�@�C�����J��
	std::ifstream ifs(path);
	if (ifs.fail()) {
		throw std::runtime_error("�V�i���I�t�@�C���̃I�[�v���Ɏ��s���܂���");
	}

	// �V�i���I�t�@�C������R�}���h��ǂݍ���Ńp�[�X����
	std::string scenario_line;
	std::vector<std::vector<std::string>> commands;
	while(std::getline(ifs, scenario_line)) {
		// UNIX��, ���ݒ�t�@�C����CRLF�̎��̑΍�
		// ��L�̏ꍇ�A���̂܂܂���csv_line�̖�����CR���t�����߁A������폜����
		if (scenario_line[scenario_line.size() - 1] == '\r') {
			scenario_line.erase(scenario_line.size() - 1);
		}

		// �����ł͕�����̃p�[�X�̂ݍs��
		// �e�R�}���h�̈�����t�H�[�}�b�g�̃`�F�b�N�͊e�R�}���h�̎��s�N���X�ōs��
		std::vector<std::string> command = CStringUtil::split(scenario_line, ' ');
		if (command.size() == 0) {
			// ��s
			continue;
		}

		commands.push_back(command);
	}

	return commands;
}
