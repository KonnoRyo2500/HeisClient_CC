/**
*	@file		log.cpp
*	@brief		heis ���O�o�̓N���X
*	@author		Ryo Konno
*	@details	���O���o�͂��邽�߂̑����񋟂���D
*/

#include "log.h"
#include "path.h"

#include <assert.h>
#include <filesystem>

// static�����o�ϐ��̒�`
std::ofstream *CLog::m_log = NULL;

/* public�֐� */

/**
*	@brief ���O�L�^���J�n����
*/
void CLog::start_logging()
{
	// ���ݓ��������O�t�@�C�����Ɋ܂܂�邽�߁A�����̃��O�t�@�C���Ɩ��O������Ă��܂����Ƃ͌����I�ɋN���肦�Ȃ�
	std::string log_file_name = "log" + build_current_datetime_str("_%Y_%m_%d_%H_%M_%S") + ".log";
	std::string log_path = join({ CC_PROJECT_DIR, "log", log_file_name });

	// ���O�t�@�C���̍쐬
	m_log = new std::ofstream(log_path, std::ios::app);
	if (m_log->fail()) {
		throw std::runtime_error("���O�L�^�̊J�n�Ɏ��s���܂���");
	}
}

/**
*	@brief ���O�L�^���I������
*/
void CLog::end_logging()
{
	delete m_log;
	m_log = NULL;
}

/**
*	@brief ���O�Ƀ��b�Z�[�W��ǉ�����
*	@param[in] level ���O�̃��x��(�d�v�x)
*	@param[in] msg �ǉ����郁�b�Z�[�W
*	@param[in] output_to_console ���O���b�Z�[�W���R���\�[���ɕ\�����邩(�f�t�H���g�ł�false(����))
*/
void CLog::write(LogLevel level, const std::string& msg, bool output_to_console)
{
	// ���O�L�^���J�n����Ă��Ȃ�
	assert(m_log != NULL);

	// ���O�̃w�b�_���쐬����
	std::string log_header = "";
	// �������ݓ���
	log_header += build_current_datetime_str("%Y/%m/%d %H:%M:%S ");

	// ���O���x��
	switch (level)
	{
	case CLog::LogLevel_Information:
		log_header += "[���]";
		break;
	case CLog::LogLevel_Warning:
		log_header += "[�x��]";
		break;
	case CLog::LogLevel_Error:
		log_header += "[�G���[]";
		break;
	default:
		log_header += "[����`!]";
		break;
	}

	// ���O�ɒǋL����
	*m_log << log_header << " " << msg << std::endl;

	// �K�v������΁A���b�Z�[�W���R���\�[���ɂ��o�͂���
	// �w�b�_������
	if (output_to_console) {
		printf("%s\n", msg.c_str());
	}
}

/**
*	@brief ���ݓ�����\����������쐬����
*	@param[in] format �������w�肷��t�H�[�}�b�g������(strftime�`��)
*	@return std::string ���ݓ�����\��������
*/
std::string CLog::build_current_datetime_str(const std::string& format)
{
	char datetime[200] = { 0 };
	std::time_t currnt_time = std::time(nullptr);

	if (strftime(datetime, sizeof(datetime), format.c_str(), std::localtime(&currnt_time)) == 0) {
		throw std::runtime_error("���O�p�̌��ݓ����̎擾�Ɏ��s���܂���");
	}
	return std::string(datetime);
}