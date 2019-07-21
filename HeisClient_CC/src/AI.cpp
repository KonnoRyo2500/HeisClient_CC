// heis ユーザAIクラス
// Author: Ryo Konno

#include "AI.h"
#include <random>

/* public関数 */

/*
	コンストラクタ
	引数1: const CCommander* commander 司令官インスタンス
*/
CUserAI::CUserAI(CCommander* commander)
{
	m_commander = commander;
}

/*
	ユーザAI メイン処理
	引数なし
	返り値なし
*/
void CUserAI::AI_main()
{
	/* この関数内に，ユーザAIの動作を記述すること */
	while (m_commander->get_all_actable_infantry_ids().size() != 0) {
		std::string infantry_id;

		if (sample_decide_action() == SampleAction_Move) {
			infantry_id = sample_select_next_infantry(m_commander->get_all_movable_infantry_ids());
			if (infantry_id.size() > 0) {
				m_commander->move(infantry_id, sample_decide_move_distance(), sample_decide_move_distance());
			}
		}
		else {
			infantry_id = sample_select_next_infantry(m_commander->get_all_attackable_infantry_ids());
			if (infantry_id.size() > 0) {
				m_commander->attack(infantry_id, sample_decide_direction());
			}
		}
	}
}

/* private関数 */

/*
	(サンプルAI用処理)兵士の移動距離を決定する関数
	引数なし
	返り値: int16_t 兵士の移動距離(-1 ~ 1)
*/
int16_t CUserAI::sample_decide_move_distance() const
{
	std::random_device rnd_dev;
	return (rnd_dev() % 3) - 1;
}

/*
	(サンプルAI用処理)次に行動する兵士をランダムに選択する関数
	引数1: const std::vector<std::string>& infantry_ids 行動可能なすべての兵士のID
	返り値: std::string 次に行動する兵士のID
*/
std::string CUserAI::sample_select_next_infantry(const std::vector<std::string>& infantry_ids) const
{
	if (infantry_ids.size() == 0) {
		return "";
	}

	std::random_device rnd_dev;
	int infantry_idx = rnd_dev() % infantry_ids.size();

	return infantry_ids[infantry_idx];
}

/*
	(サンプルAI用処理)兵士に対して命令する行動を決定する関数
	引数なし
	返り値: CUserAI::SampleAction 次の行動
*/
CUserAI::SampleAction CUserAI::sample_decide_action() const
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

/*
	(サンプルAI用処理)行動の方向を決定する関数
	引数なし
	返り値: Direction 方向
*/
Direction CUserAI::sample_decide_direction() const
{
	std::random_device rnd_dev;

	return static_cast<Direction>(rnd_dev() % 4);
}