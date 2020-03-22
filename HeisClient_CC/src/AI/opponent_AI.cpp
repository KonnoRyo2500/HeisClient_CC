/**
*	@file		opponent_AI.cpp
*	@brief		heis ローカルモード用対戦相手AIクラス
*	@author		Ryo Konno
*	@details	ローカルモードの対戦相手となるAIの思考ルーチンを定義する．
*/
#include "opponent_AI.h"
#include "field.h"
#include <random>

/* public関数 */

/**
*	@brief コンストラクタ
*	@param[in] commander 司令官インスタンス
*/
COpponentAI::COpponentAI(CCommander* commander)
{
	m_commander = commander;
}

/**
*	@brief 対戦相手AI メイン処理
*	@param[in] field_pkt 「盤面」パケット
*/
void COpponentAI::AI_main(const JSONRecvPacket_Field& field_pkt)
{
	/* この関数内に，対戦相手AIの動作を記述すること */
	std::string my_team_name = field_pkt.turn_team;

	while (m_commander->get_all_actable_infantry_ids(my_team_name).size() != 0) {
		std::string infantry_id;

		if (sample_decide_action() == SampleAction_Move) {
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

/* private関数 */

/**
*	@brief (サンプルAI用処理)兵士をランダムに移動させる関数
*	@param[in] id 行動対象の兵士のID
*/
void COpponentAI::sample_random_move(const std::string infantry_id)
{
	std::vector<FieldPosition> movable_pos = m_commander->find_movable_position(infantry_id);
	std::random_device rnd_dev;

	if (movable_pos.size() > 0) {
		FieldPosition dst_pos = movable_pos.at(rnd_dev() % movable_pos.size());

		m_commander->move(infantry_id, dst_pos);
	}
}

/**
*	@brief (サンプルAI用処理)兵士をランダムな方向に攻撃させる関数
*	@param[in] id 行動対象の兵士のID
*/
void COpponentAI::sample_random_attack(const std::string infantry_id)
{
	std::vector<FieldPosition> attackable_pos = m_commander->find_attackable_position(infantry_id);
	std::random_device rnd_dev;

	if (attackable_pos.size() > 0) {
		FieldPosition dst_pos = attackable_pos.at(rnd_dev() % attackable_pos.size());

		m_commander->attack(infantry_id, dst_pos);
	}
}

/**
*	@brief (サンプルAI用処理)次に行動する兵士をランダムに選択する関数
*	@param[in] infantry_ids 行動可能なすべての兵士のID
*	@return std::string 次に行動する兵士のID
*/
std::string COpponentAI::sample_select_next_infantry(const std::vector<std::string>& infantry_ids) const
{
	if (infantry_ids.size() == 0) {
		return "";
	}

	std::random_device rnd_dev;
	int infantry_idx = rnd_dev() % infantry_ids.size();

	return infantry_ids[infantry_idx];
}

/**
*	@brief (サンプルAI用処理)兵士に対して命令する行動を決定する関数
*	@return COpponentAI::SampleAction 次の行動
*/
COpponentAI::SampleAction COpponentAI::sample_decide_action() const
{
	std::random_device rnd_dev;
	int action_kind = rnd_dev() % 2;

	if (action_kind == 0) {
		return SampleAction_Move;
	}
	else {
		return SampleAction_Attack;
	}
}