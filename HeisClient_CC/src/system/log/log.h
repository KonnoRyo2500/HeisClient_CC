/**
*	@file		log.h
*	@brief		heis ���O�o�̓N���X
*	@author		Ryo Konno
*	@details	���O���o�͂��邽�߂̑����񋟂���D
*/
#pragma once

#include <string>
#include <fstream>

/**
*	@brief	���O�o�̓N���X
*	@remark �C���X�^���X���쐬���Ȃ��Ă����O�L�^���ł���悤�Astatic�N���X�Ƃ���
*/
class CLog
{
	// �\���́C�񋓑̂Ȃ�
	public:
		/**
		*	@enum LogLevel
		*	���O�̎��
		*/
		enum LogLevel {
			//! ���
			LogLevel_Information,
			//! �x��
			LogLevel_Warning,
			//! �G���[
			LogLevel_Error,
		};

	// �N���X�֐�
public:
	// ���O�L�^���J�n����
	static void start_logging();
	// ���O�L�^���I������
	static void end_logging();
	// ���O�Ƀ��b�Z�[�W��ǉ�����
	static void write(LogLevel level, const std::string& msg, bool output_to_console = false);

private:
	// ���ݓ�����\����������쐬����
	static std::string build_current_datetime_str(const std::string& format);

	// �N���X�ϐ�
private:
	static std::ofstream *m_log;
};