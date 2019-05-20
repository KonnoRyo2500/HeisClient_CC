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
}

/* private関数 */

/*
	(サンプルAI用処理)次に行動する兵士をランダムに選択する関数
	引数1: const std::vector<CInfantry*>& infantries 行動可能なすべての兵士
	返り値: CInfantry* 次に行動する兵士
*/
CInfantry* CUserAI::sample_select_next_infantry(const std::vector<CInfantry*>& infantries) const
{
	std::random_device rnd_dev;
	int infantry_idx = rnd_dev() % infantries.size();

	return infantries[infantry_idx];
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
	return Direction_Right;
}