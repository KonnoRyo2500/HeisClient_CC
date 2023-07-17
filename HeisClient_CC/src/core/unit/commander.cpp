/**
*	@file		commander.cpp
*	@brief		heis �i�ߊ��N���X
*	@author		Ryo Konno
*	@details	�`�[���̕��m���ꌳ�Ǘ����A������Ԏ擾���s���B
*/

#include "commander.h"

#include <iterator>
#include <algorithm>

/**
*	@brief �R���X�g���N�^
*	@param[in] team_name �`�[����
*	@param[in] board �Ֆ�
*/
CCommander::CCommander(const std::string& team_name, CBoard* board)
	: m_team_name(team_name)
	, m_board(board)
	, m_observer(CBoardObserver())
	, m_controller(CBoardController())
{
	// Do Nothing
}

/**
*	@brief �w�肵��ID�������m�̈ʒu���擾����
*	@param[in] id ���mID
*	@return Coordinate2D ���m�̈ʒu(�w�肵��ID�̕��m��������Ȃ��ꍇ��INVALID_POSITION)
*/
Coordinate2D CCommander::get_infantry_position_by_id(const std::string& id) const
{
	std::vector<InfantryWithPos> infantries_and_pos = m_observer.fetch_all_infantry_and_position(*m_board);
	for (auto& ip : infantries_and_pos) {
		if (ip.first.get_status().id == id){
			return ip.second;
		}
	}

	return INVALID_COODINATE;
}

/**
*	@brief �w�肵�����m�̃X�e�[�^�X���擾����
*	@param[in] id ���mID
*	@return InfantryStatus ���m�̃X�e�[�^�X
*/
InfantryStatus CCommander::get_infantry_status_by_id(const std::string& id) const
{
	CInfantry infantry = find_infantry_by_id(id).first;
	return infantry.get_status();
}

/**
*	@brief ���m�ɍU�����w������
*	@param[in] id ���m��ID
*	@param[in] dst �U����̍��W
*/
void CCommander::attack(const std::string& id, const Coordinate2D& dst)
{
	InfantryWithPos infantry_and_pos = find_infantry_by_id(id);
	CInfantry infantry = infantry_and_pos.first;
	Coordinate2D pos = infantry_and_pos.second;
	m_controller.attack(m_board, m_observer, pos, dst);

	// �̂��Ɂu�s���v�p�P�b�g���쐬�ł���悤�ɂ��邽�߁A�U������L�^���Ă���
	m_builder.add_attack_destination(infantry, dst);
}

/**
*	@brief ���m�Ɉړ����w������
*	@param[in] id ���m��ID
*	@param[in] dst �ړ���̍��W
*/
void CCommander::move(const std::string& id, const Coordinate2D& dst) const
{
	InfantryWithPos infantry_and_pos = find_infantry_by_id(id);
	Coordinate2D pos = infantry_and_pos.second;
	m_controller.move(m_board, m_observer, pos, dst);
}

/**
*	@brief �ړ��\�Ȃ��ׂẴ}�X���擾����
*	@param[in] id ���m��ID
*	@return std::vector<Coordinate2D> �ړ��\�ȃ}�X
*/
std::vector<Coordinate2D> CCommander::find_movable_position(const std::string& id) const
{
	Coordinate2D pos = find_infantry_by_id(id).second;
	return m_observer.search_position_to_move(*m_board, pos);
}

/**
*	@brief �U���\�Ȃ��ׂẴ}�X���擾����
*	@param[in] id ���m��ID
*	@return std::vector<Coordinate2D> �U���\�ȃ}�X
*/
std::vector<Coordinate2D> CCommander::find_attackable_position(const std::string& id) const
{
	Coordinate2D pos = find_infantry_by_id(id).second;
	return m_observer.search_position_to_attack(*m_board, pos);
}

/**
*	@brief �s���\�ȕ��m��ID���擾����
*	@details �u�s���\�v�Ƃ́A�ړ��������͍U�����ł���Ƃ������Ƃ��w���B
*	@param[in] team_name ���mID�擾�Ώۂ̃`�[����
*	@return std::vector<std::string> �s���\�ȕ��m��ID
*/
std::vector<std::string> CCommander::get_all_actable_infantry_ids(const std::string& team_name) const
{
	std::vector<std::string> movable_ids = get_all_movable_infantry_ids(team_name);
	std::vector<std::string> attackable_ids = get_all_attackable_infantry_ids(team_name);
	std::vector<std::string> actable_ids;

	// �ړ��\�A�܂��͍U���\�ȕ��m��ID��T��
	std::copy(movable_ids.begin(), movable_ids.end(), std::back_inserter(actable_ids));
	for (auto& at_id : attackable_ids) {
		if (std::find(actable_ids.begin(), actable_ids.end(), at_id) != actable_ids.end()) {
			actable_ids.push_back(at_id);
		}
	}

	return actable_ids;
}

/**
*	@brief �ړ��\�ȕ��m��ID���擾����
*	@param[in] team_name ���mID�擾�Ώۂ̃`�[����
*	@return std::vector<std::string> �ړ��\�ȕ��m��ID
*/
std::vector<std::string> CCommander::get_all_movable_infantry_ids(const std::string& team_name) const
{
	std::vector<InfantryWithPos> infantries_and_pos = m_observer.fetch_all_infantry_and_position(*m_board);
	std::vector<std::string> ids;
	for (auto& ip : infantries_and_pos) {
		CInfantry infantry = ip.first;
		Coordinate2D pos = ip.second;

		// �w�肳�ꂽ�`�[�����ŁA���ړ���̍��W�����݂���ꍇ�ړ��\
		bool has_team_name = (infantry.get_status().team_name == team_name);
		bool has_pos_to_move = (m_observer.search_position_to_move(*m_board, pos).size() >= 1);
		if (has_team_name && has_pos_to_move) {
			ids.push_back(infantry.get_status().id);
		}
	}

	return ids;
}

/**
*	@brief �U���\�ȕ��m��ID���擾����
*	@param[in] team_name ���mID�擾�Ώۂ̃`�[����
*	@return std::vector<std::string> �U���\�ȕ��m��ID
*/
std::vector<std::string> CCommander::get_all_attackable_infantry_ids(const std::string& team_name) const
{
	std::vector<InfantryWithPos> infantries_and_pos = m_observer.fetch_all_infantry_and_position(*m_board);
	std::vector<std::string> ids;
	for (auto& ip : infantries_and_pos) {
		CInfantry infantry = ip.first;
		Coordinate2D pos = ip.second;

		// �w�肳�ꂽ�`�[�����ŁA���U����̍��W�����݂���ꍇ�U���\
		bool has_team_name = (infantry.get_status().team_name == team_name);
		bool has_pos_to_attack = (m_observer.search_position_to_attack(*m_board, pos).size() >= 1);
		if (has_team_name && has_pos_to_attack) {
			ids.push_back(infantry.get_status().id);
		}
	}

	return ids;
}

/**
*	@brief �u�s���v�p�P�b�g���쐬����
*	@return JSONSendPacket_Action �u�s���v�p�P�b�g
*/
JSONSendPacket_Action CCommander::create_action_pkt()
{
	JSONSendPacket_Action action_pkt = m_builder.build(*m_board, m_team_name);
	m_builder.clear();
	return action_pkt;
}

/**
*	@brief ID���畺�m�ƈʒu���擾����
*	@param[in] id ���mID
*	@return InfantryWithPos ���m�̎��̂Ƃ��̈ʒu
*/
InfantryWithPos CCommander::find_infantry_by_id(const std::string& id) const
{
	std::vector<InfantryWithPos> infantries_and_pos = m_observer.fetch_all_infantry_and_position(*m_board);
	for (auto& ip : infantries_and_pos) {
		if (ip.first.get_status().id == id) {
			return ip;
		}
	}

	return std::make_pair(CInfantry(InfantryStatus("", "")), INVALID_COODINATE);
}