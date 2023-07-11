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

/* public�֐� */
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
	initialize_watch(setting);

	// �T�[�o�����M�����������\��("finished"�͑ΐ�I����\��������)
	std::string msg = "";
	do {
		msg = m_sck->recv();
		printf("%s\n", msg.c_str());
	} while (msg != "finished");

	// �ϐ�I��
	finalize_watch();

	CLog::write(CLog::LogLevel_Information, "�ϐ킪�I�����܂���");
}

/* private�֐� */

/**
*	@brief �ϐ�̏��������s���֐�
*	@param[in] setting �ϐ탂�[�h�ݒ�l
*/
void CAudienceMode::initialize_watch(const AudienceSetting& setting)
{
	// �T�[�o�ɐڑ�
	m_sck = new CSocket();
	m_sck->connect(setting.server_ip_addr, setting.server_port_num);
}

/**
*	@brief �ϐ�̏I���������s���֐�
*/
void CAudienceMode::finalize_watch()
{
	// �C���X�^���X��j��
	delete m_sck;
	m_sck = NULL;
}