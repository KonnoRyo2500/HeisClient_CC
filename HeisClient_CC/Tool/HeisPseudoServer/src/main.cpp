/**
*	@file		main.cpp
*	@brief		PS ���C������
*	@author		Ryo Konno
*	@details	PS�̃��C���������`����B
*/

#include "path.h"
#include "ps_setting_file.h"
#include "socket.h"
#include "scenario_file.h"
#include "command_executor_factory.h"

#include <stdexcept>

/**
*	@def PS_SETTING_FILE_NAME
*	@brief PS�ݒ�t�@�C���̖��O
*/
#define PS_SETTING_FILE_NAME "ps_setting.csv"

/**
*	@brief ���C������
*/
int main()
{
	try{
		// �ݒ�t�@�C����ǂݍ���
		PsSetting setting = CPsSettingFile().load(
			join({PS_SETTING_DIR, PS_SETTING_FILE_NAME})
		);

		// ��U�N���C�A���g����̐ڑ����󂯕t����
		CSocket socket_first, socket_second;
		printf(
			"CC��IP�A�h���X: %s, �|�[�g�ԍ�: %d�ɐڑ����Ă�������\n",
			setting.listen_addr_first.c_str(),
			setting.listen_port_num_first
		);
		socket_first.bind(
			setting.listen_port_num_first,
			setting.listen_addr_first
		);
		socket_first.listen();
		socket_first.accept();

		// ��U�N���C�A���g����̐ڑ����󂯕t����
		printf(
			"CC��IP�A�h���X: %s, �|�[�g�ԍ�: %d�ɐڑ����Ă�������\n",
			setting.listen_addr_second.c_str(),
			setting.listen_port_num_second
		);
		socket_second.bind(
			setting.listen_port_num_second,
			setting.listen_addr_second
		);
		socket_second.listen();
		socket_second.accept();

		// �V�i���I�t�@�C������R�}���h��ǂݍ���
		printf("�R�}���h���s�J�n\n");
		std::vector<std::vector<std::string>> commands = CScenarioFile().load(
			join({PS_SCENARIO_DIR, setting.scenario_file_name})
		);

		// �e�R�}���h�̎��s
		CCommandExecutorFactory factory = CCommandExecutorFactory();
		for (auto& cmd : commands) {
			CCommandExecutorBase* executor = factory.create_instance(
				cmd[0],
				socket_first,
				socket_second
			);

			if (executor == NULL) {
				fprintf(stderr, "�R�}���h %s �͖���`�ł�\n", cmd[0].c_str());
				continue;
			}

			executor->execute(cmd);

			delete executor;
			executor = NULL;
		}
		printf("�R�}���h�̎��s���������܂���\n");
	}
	catch(std::exception& e){
		fprintf(stderr, "PS�̎��s���ɃG���[���������܂���(���e: %s)\n", e.what());
		return EXIT_FAILURE;
	}

	return EXIT_SUCCESS;
}
