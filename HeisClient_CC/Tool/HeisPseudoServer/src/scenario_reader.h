// heis�N���C�A���g�pJSON����M���� �V�i���I�ǂݍ��݃N���X
// Author: Ryo Konno

#include <string>

class CScenarioReader
{
	// �\���́E�񋓑̂Ȃ�
	public:
		// �A�N�V�����̎��
		enum ActionType {
			// ��M: �N���C�A���g���烁�b�Z�[�W����M����
			ActionType_Receive,
			// ���b�Z�[�W���M: �V�i���I�t�@�C���ɋL�ڂ��ꂽ���b�Z�[�W���N���C�A���g�ɑ��M����
			ActionType_SendMessage,
			// �t�@�C�����M: �V�i���I�t�@�C���Ŏw�肳�ꂽ�t�@�C���̓��e���N���C�A���g�ɑ��M����
			ActionType_SendFileContents,
			// �S�A�N�V�������s����: �V�i���I�t�@�C���ɋL�ڂ��ꂽ�A�N�V���������ׂĎ��s����
			ActionType_AllActionDone,
		};

	// �����o�֐�
	public:
		// �R���X�g���N�^
		explicit CScenarioReader(const std::string& scenario_file_name);
		// �f�X�g���N�^
		~CScenarioReader();

		// ���̃A�N�V�����̎�ނ��擾
		ActionType get_next_aciton_type();

		// ���M���郁�b�Z�[�W��t�@�C�������擾
		std::string get_message_to_send();
		std::string get_filename_to_send();

	private:

	// �����o�ϐ�
	private:
		// ���̃A�N�V����
		ActionType m_next_action_type;
		// �V�i���I�t�@�C��
		FILE* m_scenatio_file;
};
