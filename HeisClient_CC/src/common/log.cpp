// heis ���O�o�̓N���X
// Author: Ryo Konno
#include "log.h"
#include "heis_client_exception.h"
#include "path_generator.h"

#include <ctime>
#include <locale>
#include <iostream>
#include <map>

/* public�֐� */

/*
	�R���X�g���N�^
	����1: const std::string& log_name ���O�t�@�C����
	����2: const bool add_datetime_to_name ���O�t�@�C�����̖����Ɍ��ݓ��������邩(�f�t�H���g��true)
*/
CLog::CLog(const std::string& log_name, const bool add_datetime_to_name)
{
	// "bin"�f�B���N�g���Ɠ����"log"�f�B���N�g���Ƀ��O���o�͂���
	// Windows��Linux�Ńv���W�F�N�g�f�B���N�g���̍\�����قȂ�̂ŁC����ɍ��킹�ă��O�o�͐��
	// �p�X���ς���
#ifdef WIN32
	std::string actual_log_name = CPathGenerator::get_exe_path() + "..\\..\\log\\" + log_name;
#else
	std::string actual_log_name = CPathGenerator::get_exe_path() + "../log/" + log_name;
#endif // WIN32

	if (add_datetime_to_name) {
		actual_log_name += make_current_datetime_str("%Y_%m_%d_%H_%M_%S");
	}
	actual_log_name += ".log";

	m_logfile = new std::ofstream(actual_log_name);
	if (m_logfile->fail()) {
		throw CHeisClientException("���O�t�@�C���̃I�[�v���Ɏ��s���܂���(�t�@�C����: %s)", actual_log_name.c_str());
	}
}

/*
	�f�X�g���N�^
	�����Ȃ�
*/
CLog::~CLog()
{
	delete m_logfile;
	m_logfile = NULL;
}

/*
	���O�Ƀ��b�Z�[�W���������ފ֐�
	����1: const LogType log_type ���O�̎��
	����2: const std::string& message �������ރ��b�Z�[�W
	�Ԃ�l�Ȃ�
*/
void CLog::write_log(const LogType log_type, const std::string& message) const
{
	*m_logfile <<
		make_current_datetime_str("%Y/%m/%d %H:%M:%S ") <<
		"[" + make_log_type_str(log_type) + "] " <<
		message <<
		std::endl;
}

/* private�֐� */

/*
	���ݓ�����\���������Ԃ��֐�
	����1: const std::string& format �������w�肷��t�H�[�}�b�g������(strftime�`��)
	�Ԃ�l: std::string ���ݓ�����\��������
*/
std::string CLog::make_current_datetime_str(const std::string& format) const
{
	char datetime[200] = { 0 };
	std::time_t currnt_time = std::time(nullptr);

	if (strftime(datetime, sizeof(datetime), format.c_str(), std::localtime(&currnt_time)) == 0) {
		throw CHeisClientException("���O�p�̌��ݓ����̎擾�Ɏ��s���܂���");
	}
	return std::string(datetime);
}

/*
	���O�̎�ނ�\����������쐬����֐�
	����1: const LogType type ���O�̎��
	�Ԃ�l: std::string ���O�̎�ނ�\��������
*/
std::string CLog::make_log_type_str(const LogType type) const
{
	// ���O�̎�� -> ������̑Ή��\
	const std::map<LogType, std::string> log_type_map = {
		{LogType_Infomation,	"���"},
		{LogType_Warning,		"�x��"},
		{LogType_Error,			"�G���["},
	};

	auto it = log_type_map.find(type);
	if (it != log_type_map.end()) {
		return it->second;
	}

	return "����`�̃��O";
}