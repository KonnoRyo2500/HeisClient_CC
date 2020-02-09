// heis ���O�o�̓N���X
// Author: Ryo Konno
#pragma once

#include <string>
#include <fstream>

class CLog
{
	// �\���́C�񋓑̂Ȃ�
	public:
		// ���O�̎��
		enum LogType {
			// ���
			LogType_Infomation,
			// �x��
			LogType_Warning,
			// �G���[
			LogType_Error,
		};

	// �����o�֐�
	public:
		// �R���X�g���N�^
		CLog(const std::string& log_name, const bool add_datetime_to_name = true);
		// �f�X�g���N�^
		~CLog();

		// ���O�ɏ�������
		void write_log(const LogType log_type, const std::string& message) const;

	private:
		// ���ݓ�����\����������擾
		std::string get_current_datetime_str(const std::string& format) const;

	// �����o�ϐ�
	private:
		std::ofstream* m_logfile;
};