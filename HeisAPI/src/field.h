// heis �t�B�[���h�N���X
// Author: Ryo Konno
#pragma once

#include "infantry.h"
#include <array>

class CField
{
	// �\���́C�񋓑̂Ȃ�
	private:
		// �t�B�[���h�Ɋւ���萔
		enum FieldParam {
			FieldParam_Width = 20,				// ��
			FieldParam_Height = 20,				// ����
		};

	// �����o�ϐ�
	private:
		std::array<CInfantry*, FieldParam_Width * FieldParam_Height> m_field;		// �t�B�[���h�̊e�}�X��

	// �����o�֐�
	public:
		CInfantry* get_infantry(uint16_t pos_x, uint16_t pos_y);
		void set_infantry(uint16_t pos_x, uint16_t pos_y, CInfantry* infantry);
		void end_your_turn();
};