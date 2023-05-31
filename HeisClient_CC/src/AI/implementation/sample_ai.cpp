/**
*	@file		sample_ai.cpp
*	@brief		heis サンプルAI
*	@author		Ryo Konno
*	@details	AIのサンプル実装。ランダムに行動を選択する。
*/

#include "sample_ai.h"

#include <random>

/* public */

/**
*	@brief コンストラクタ
*	@param[in] commander 司令官インスタンス
*/
CSampleAI::CSampleAI(CCommander* commander)
	: CAIBase(commander)
{
	// Do Nothing
}

/**
*	@brief AIのメイン処理
*	@param[in] board_pkt 「盤面」パケット
*/
void CSampleAI::AI_main(const JSONRecvPacket_Board& board_pkt)
{
	std::string my_team_name = board_pkt.turn_team.get_value();

	while (m_commander->get_all_actable_infantry_ids(my_team_name).size() != 0) {
		std::string infantry_id;

		if (sample_decide_action() == Action_Move) {
			infantry_id = sample_select_next_infantry(m_commander->get_all_movable_infantry_ids(my_team_name));
			if (infantry_id.size() > 0) {
				sample_random_move(infantry_id);
			}
		}
		else {
			infantry_id = sample_select_next_infantry(m_commander->get_all_attackable_infantry_ids(my_team_name));
			if (infantry_id.size() > 0) {
				sample_random_attack(infantry_id);
			}
		}
	}
}

/* private */

/**
*	@brief 兵士をランダムに移動させる関数
*	@param[in] infantry_id 行動対象の兵士のID
*/
void CSampleAI::sample_random_move(const std::string infantry_id)
{
	std::vector<BoardPosition> movable_pos = m_commander->find_movable_position(infantry_id);
	std::random_device rnd_dev;

	if (movable_pos.size() > 0) {
		BoardPosition dst_pos = movable_pos.at(rnd_dev() % movable_pos.size());

		m_commander->move(infantry_id, dst_pos);
	}
}

/**
*	@brief 兵士をランダムな方向に攻撃させる関数
*	@param[in] infantry_id 行動対象の兵士のID
*/
void CSampleAI::sample_random_attack(const std::string infantry_id)
{
	std::vector<BoardPosition> attackable_pos = m_commander->find_attackable_position(infantry_id);
	std::random_device rnd_dev;

	if (attackable_pos.size() > 0) {
		BoardPosition dst_pos = attackable_pos.at(rnd_dev() % attackable_pos.size());

		m_commander->attack(infantry_id, dst_pos);
	}
}

/**
*	@brief 次に行動する兵士をランダムに選択する関数
*	@param[in] infantry_ids 行動可能なすべての兵士のID
*	@return std::string 次に行動する兵士のID
*/
std::string CSampleAI::sample_select_next_infantry(const std::vector<std::string>& infantry_ids) const
{
	if (infantry_ids.size() == 0) {
		return "";
	}

	std::random_device rnd_dev;
	int infantry_idx = rnd_dev() % infantry_ids.size();

	return infantry_ids[infantry_idx];
}

/**
*	@brief 兵士に対して命令する行動を決定する関数
*	@return CSampleAI::Action 次の行動
*/
CSampleAI::Action CSampleAI::sample_decide_action() const
{
	std::random_device rnd_dev;
	int action_kind = rnd_dev() % 2;

	if (action_kind == 0) {
		return Action_Move;
	}
	else {
		return Action_Attack;
	}
}