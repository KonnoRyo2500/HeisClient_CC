/**
*	@file		audience_mode.cpp
*	@brief		heis �ϐ탂�[�h�N���X
*	@author		Ryo Konno
*	@details	heis�̑ΐ���ϐ킷��D
*/

#include "audience_mode.h"
#include "setting_keys.h"
#include "csv_setting_file_reader.h"
#include "path_generator.h"

/**
*	@def AUDIENCE_SETTING_FILE_NAME
*	@brief �ϐ탂�[�h�ݒ�t�@�C���̖��O
*/
#define AUDIENCE_SETTING_FILE_NAME "audience_setting.csv"

/* public�֐� */
/**
*	@brief �ϐ탁�C������
*/
void CAudienceMode::watch_game()
{
	g_system_log.write_log(CLog::LogType_Infomation, false, "�ϐ���J�n���܂���");

	// �ϐ�J�n
	initialize_watch();

	// �T�[�o�����M�����������\��("finished"�͑ΐ�I����\��������)
	std::string msg = "";
	do {
		msg = m_sck->sck_recv();
		printf("%s\n", msg.c_str());
	} while (msg != "finished");

	// �ϐ�I��
	finalize_watch();

	g_system_log.write_log(CLog::LogType_Infomation, false, "�ϐ킪�I�����܂���");
}

/* private�֐� */

/**
*	@brief �ϐ�̏��������s���֐�
*/
void CAudienceMode::initialize_watch()
{
	CCsvSettingFileReader sr(CPathGenerator::get_exe_path() + AUDIENCE_SETTING_FILE_NAME);
	std::string svr_addr;
	uint16_t svr_port;

	// �C���X�^���X�𐶐�
	m_sck = new CClientSocket();

	// �ڑ�����T�[�o�̏���ݒ�t�@�C������擾
	svr_addr = sr.get_single_value<std::string>(AUDIENCE_SETTING_KEY_SVR_ADDR, 0);
	svr_port = sr.get_single_value<uint16_t>(AUDIENCE_SETTING_KEY_SVR_PORT, 0);

	// �T�[�o�ɐڑ�
	m_sck->sck_connect(svr_addr, svr_port);
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