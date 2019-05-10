// heis ���m�N���X
// Author: Ryo Konno
#include "infantry.h"

/* public�֐� */

/*
	�R���X�g���N�^
	����2: int infantry_ID ���mID
*/
CInfantry::CInfantry(const std::string& team_name, const uint16_t infantry_ID, const uint16_t init_pos_x, uint16_t init_pos_y)
	: m_id(infantry_ID)
	, m_hp(InitialValue_HP)
	, m_team_name(team_name)
	, m_pos_x(init_pos_x)
	, m_pos_y(init_pos_y)
{
	// �����Ȃ�
}

/*
	���m��HP���擾����֐�
	�����Ȃ�
	�Ԃ�l: uint8_t ���m��HP
*/
uint8_t CInfantry::get_hp()
{
	return m_hp;
}

/*
	���m��x���W���擾����֐�
	�����Ȃ�
	�Ԃ�l: uint16_t ���m��x���W
*/
uint16_t CInfantry::get_x_position()
{
	return m_pos_x;
}

/*
	���m��y���W���擾����֐�
	�����Ȃ�
	�Ԃ�l: Position ���m��y���W
*/
uint16_t CInfantry::get_y_position()
{
	return m_pos_y;
}

/*
	�w�肵�������ɍU�����s���֐�
	����1: Direction direction �U������
	�Ԃ�l�Ȃ�
*/
void CInfantry::attack(Direction direction)
{
	
}

/*
	�w�肵�������ɕ��m���ړ�����֐�
	����1: Direction direction �ړ�����
	�Ԃ�l�Ȃ�
*/
void CInfantry::move(Direction direction)
{

}

/* private�֐� */

/*
	�G�̕��m����̍U���𔽉f������֐�
	�����Ȃ�
	�Ԃ�l�Ȃ�
	����: ���̊֐��́C�����I�ȏ������s�����߂̂��̂Ȃ̂ŁC���[�UAI�͂��̊֐����Ă�ł͂����Ȃ�
*/
void CInfantry::attacked()
{
	m_hp--;
}