// heis ���m�N���X
// Author: Ryo Konno
#pragma once

#include <string>

class CInfantry
{
	// �\���́C�񋓑̂Ȃ�
	public:
		// ����
		enum Direction {
			Direction_Up,
			Direction_Right,
			Direction_Down,
			Direction_Left,
		};

	private:
		// �����l(�萔)
		enum InitialValue {
			InitialValue_HP = 2,
		};

	// �����o�֐�
	public:
		// �R���X�g���N�^
		explicit CInfantry(const std::string& team_name, const uint16_t infantry_ID, const uint16_t init_pos_x, uint16_t init_pos_y);

		// �X�e�[�^�X�̎擾
		uint8_t get_hp();
		uint16_t get_x_position();
		uint16_t get_y_position();

		// �s��
		void attack(Direction direction);
		void move(Direction direction);

		// ���������p(���[�U��`AI�ł͌Ă΂Ȃ�����)
		void attacked();

	private:

	// �����o�ϐ�
	private:
		std::string m_team_name;
		uint16_t m_id;
		uint16_t m_pos_x;
		uint16_t m_pos_y;
		uint8_t m_action_remain;
		uint8_t m_hp;
};