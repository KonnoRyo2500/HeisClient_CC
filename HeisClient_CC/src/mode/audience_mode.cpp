/**
*	@file		audience_mode.cpp
*	@brief		heis �ϐ탂�[�h�N���X
*	@author		Ryo Konno
*	@details	heis�̑ΐ���ϐ킷��D
*/

#include "audience_mode.h"
#include "setting_keys.h"
#include "audience_setting_file.h"
#include "path.h"

/**
*	@def AUDIENCE_SETTING_FILE_NAME
*	@brief �ϐ탂�[�h�ݒ�t�@�C���̖��O
*/
#define AUDIENCE_SETTING_FILE_NAME "audience_setting.csv"

/**
*	@brief �ϐ탁�C������
*/
void CAudienceMode::play_game()
{
	CLog::write(CLog::LogLevel_Information, "�ϐ���J�n���܂���");

	// �ݒ�t�@�C���̓ǂݍ���
	AudienceSetting setting = CAudienceSettingFile().load(
		join({CC_SETTING_DIR, AUDIENCE_SETTING_FILE_NAME})
	);

	// �ϐ�J�n
	CSocket socket;
	socket.connect(setting.server_ip_addr, setting.server_port_num);

	// �T�[�o�����M�����������\��("finished"�͑ΐ�I����\��������)
	std::string msg = "";
	do {
		msg = socket.recv('\n');
		printf("%s\n", msg.c_str());
	} while (msg != "finished");

	CLog::write(CLog::LogLevel_Information, "�ϐ킪�I�����܂���");
}