// heis �i�ߊ��N���X
// Author: Ryo Konno
#pragma once

#include "infantry.h"
#include <array>

class CCommander
{
	// �\���́C�񋓑̂Ȃ�
	private:
		// �����l
		enum InitialValue {
			InitialValue_NumInfantry = 30,			// ���m�̐l��
		};

	// �����o�֐�
	public:
		// �R���X�g���N�^
		explicit CCommander(std::string team_name);

	// �����o�ϐ�
	private:
		std::string m_team_name;
		std::array<CInfantry*, InitialValue_NumInfantry> m_infantries;		// �`�[���̑S���m
};