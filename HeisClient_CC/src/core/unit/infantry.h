/**
*	@file		infantry.h
*	@brief		heis ���m�N���X
*	@author		Ryo Konno
*	@details	heis�̃��j�b�g��1�ł��镺�m���j�b�g���`����D
*/
#pragma once

#include "const_val.h"
#include "common_structure.h"
#include "JSON_data_packet.h"

#include <string>
#include <vector>

/**
*	@struct InfantryStatus
*	@brief ���m�̃X�e�[�^�X
*/
struct InfantryStatus
{
	/**
	*	@brief �R���X�g���N�^
	*	@param[in] team_name �`�[����
	*	@param[in] id ID
	*	@param[in] hp ����HP
	*	@param[in] action_remain �����c��s����
	*/
	explicit InfantryStatus(
		const std::string& team_name,
		const std::string& id,
		uint8_t hp = INFANTRY_INITIAL_HP,
		uint8_t action_remain = INFANTRY_ACTION_LIMIT
	)
		: team_name(team_name)
		, id(id)
		, action_remain(action_remain)
		, hp(hp)
	{
		// Do Nothing
	}

	//! �`�[����
	std::string team_name;
	//! ID
	std::string id;
	//! �c��s����
	uint8_t action_remain;
	//! HP
	uint8_t hp;
};

/**
*	@brief	���m�N���X
*/
class CInfantry
{
	// �����o�֐�
	public:
		// �R���X�g���N�^
		explicit CInfantry(const InfantryStatus& init_status);

		// �f�X�g���N�^
		~CInfantry();

		// �X�e�[�^�X���擾����
		InfantryStatus get_status() const;

		// HP��1���炷
		void reduce_hp();
		// �s���񐔂��w�肳�ꂽ�񐔌��炷
		void reduce_remaining_action(const uint8_t n_reduce);

		// ����ł��邩�ǂ����̔���
		bool is_dead() const;

	// �����o�ϐ�
	private:
		//! ���݂̃X�e�[�^�X
		InfantryStatus m_status;
};