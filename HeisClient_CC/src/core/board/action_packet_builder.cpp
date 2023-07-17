/**
*	@file		action_packet_builder.cpp
*	@brief		heis �u�s���v�p�P�b�g�쐬�N���X
*	@author		Ryo Konno
*	@details	�Ֆʏ�̕��m����u�s���v�p�P�b�g���쐬����B
*/

#include "action_packet_builder.h"

/**
*	@brief �u�s���v�p�P�b�g���쐬����
*	@param[in] board �Ֆ�
*	@param[in] team_name �`�[����
*	@return JSONSendPacket_Action �u�s���v�p�P�b�g
*/
JSONSendPacket_Action CActionPacketBuilder::build(const CBoard& board, const std::string& team_name) const
{
	JSONSendPacket_Action action_pkt;

	action_pkt.turn_team.set_value(team_name);

	// contents
	BoardSize size = board.get_size();
	for (size_t y = 0; y < size.height; y++) {
		for (size_t x = 0; x < size.width; x++) {
			Square sq = board.get_square(Coordinate2D(x, y));
			if (!sq.exists) {
				continue;
			}

			ContentsArrayElem elem;
			std::string id = sq.infantry.get_status().id;
			Coordinate2D atk_dst = find_attack_destination_if_attacked(id);
			elem.unit_id.set_value(id);
			elem.to_x.set_value((uint16_t)x);
			elem.to_y.set_value((uint16_t)y);
			if (atk_dst != INVALID_COODINATE) {
				elem.atk_x.set_value(atk_dst.x);
				elem.atk_y.set_value(atk_dst.y);
			}
		}
	}

	return action_pkt;
}

/**
*	@brief ���m���U���������W��ǉ�����
*	@param[in] infantry �U���������m
*	@param[in] atk_dst �U������W
*/
void CActionPacketBuilder::add_attack_destination(const CInfantry& infantry, const Coordinate2D& atk_dst)
{
	std::string id = infantry.get_status().id;
	m_attack_destinations.push_back(std::make_pair(id, atk_dst));
}

/**
*	@brief ������Ԃ�����������
*/
void CActionPacketBuilder::clear()
{
	m_attack_destinations.clear();
}

/**
*	@brief �������m���U���ς݂Ȃ�΁A�U����̍��W��Ԃ�
*	@param[in] id ���mID
*	@return Coordinate2D �U����̍��W(�U���ς݂łȂ����INVALID_POSITION)
*/
Coordinate2D CActionPacketBuilder::find_attack_destination_if_attacked(const std::string& id) const
{
	for (auto& dst : m_attack_destinations) {
		if (dst.first == id) {
			return dst.second;
		}
	}

	return INVALID_COODINATE;
}