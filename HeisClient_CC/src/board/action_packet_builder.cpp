/**
*	@file		action_packet_builder.cpp
*	@brief		heis 「行動」パケット作成クラス
*	@author		Ryo Konno
*	@details	盤面上の兵士から「行動」パケットを作成する。
*/

#include "action_packet_builder.h"

/**
*	@brief 「行動」パケットを作成する
*	@param[in] board 盤面
*	@param[in] team_name チーム名
*	@return JSONSendPacket_Action 「行動」パケット
*/
JSONSendPacket_Action CActionPacketBuilder::build(const CBoard& board, const std::string& team_name) const
{
	JSONSendPacket_Action action_pkt;

	action_pkt.turn_team.set_value(team_name);

	// contents
	BoardSize size = board.get_size();
	for (size_t y = 0; y < size.height; y++) {
		for (size_t x = 0; x < size.width; x++) {
			Square sq = board.get_square(BoardPosition(x, y));
			if (!sq.exists) {
				continue;
			}

			ContentsArrayElem elem;
			std::string id = sq.infantry.get_status().id;
			BoardPosition atk_dst = find_attack_destination_if_attacked(id);
			elem.unit_id.set_value(id);
			elem.to_x.set_value((uint16_t)x);
			elem.to_y.set_value((uint16_t)y);
			if (atk_dst != INVALID_POSITION) {
				elem.atk_x.set_value(atk_dst.x);
				elem.atk_y.set_value(atk_dst.y);
			}
		}
	}

	return action_pkt;
}

/**
*	@brief 兵士が攻撃した座標を追加する
*	@param[in] infantry 攻撃した兵士
*	@param[in] atk_dst 攻撃先座標
*/
void CActionPacketBuilder::add_attack_destination(const CInfantry& infantry, const BoardPosition& atk_dst)
{
	std::string id = infantry.get_status().id;
	m_attack_destinations.push_back(std::make_pair(id, atk_dst));
}

/**
*	@brief 内部状態を初期化する
*/
void CActionPacketBuilder::clear()
{
	m_attack_destinations.clear();
}

/**
*	@brief もし兵士が攻撃済みならば、攻撃先の座標を返す
*	@param[in] id 兵士ID
*	@return BoardPosition 攻撃先の座標(攻撃済みでなければINVALID_POSITION)
*/
BoardPosition CActionPacketBuilder::find_attack_destination_if_attacked(const std::string& id) const
{
	for (auto& dst : m_attack_destinations) {
		if (dst.first == id) {
			return dst.second;
		}
	}

	return INVALID_POSITION;
}