// heis 司令官クラス
// Author: Ryo Konno

#include "commander.h"

/* public関数 */

/*
	コンストラクタ
	引数1: const std::string& team_name チーム名
*/
CCommander::CCommander(const std::string& team_name)
	: m_team_name(team_name)
{
	// 処理なし(あれば初期化処理)
}

/*
	指定したIDを持つ兵士のx座標を取得する関数
	引数1: (const std::string& id ID
	返り値: int16_t 兵士のx座標(指定したIDの兵士がいなければ-1)
*/
int16_t CCommander::get_x_position(const std::string& id) const
{
	CInfantry* infantry = search_infantry_by_id(id);

	if (infantry != NULL) {
		return infantry->get_x_position();
	}
	else {
		return INFANTRY_STATUS_ERROR;
	}
}

/*
	指定したIDを持つ兵士のy座標を取得する関数
	引数1: (const std::string& id ID
	返り値: int16_t 兵士のy座標(指定したIDの兵士がいなければ-1)
*/
int16_t CCommander::get_y_position(const std::string& id) const
{
	CInfantry* infantry = search_infantry_by_id(id);

	if (infantry != NULL) {
		return infantry->get_y_position();
	}
	else {
		return INFANTRY_STATUS_ERROR;
	}
}

/*
	指定したIDを持つ兵士の残り行動回数を取得する関数
	引数1: (const std::string& id ID
	返り値: int16_t 兵士の残り行動回数(指定したIDの兵士がいなければ-1)
*/
int8_t CCommander::get_action_remain(const std::string& id) const
{
	CInfantry* infantry = search_infantry_by_id(id);

	if (infantry != NULL) {
		return infantry->get_action_remain();
	}
	else {
		return INFANTRY_STATUS_ERROR;
	}
}

/*
	指定したIDを持つ兵士のHPを取得する関数
	引数1: (const std::string& id ID
	返り値: int16_t 兵士のHP(指定したIDの兵士がいなければ-1)
*/
int8_t CCommander::get_hp(const std::string& id) const
{
	CInfantry* infantry = search_infantry_by_id(id);

	if (infantry != NULL) {
		return infantry->get_hp();
	}
	else {
		return INFANTRY_STATUS_ERROR;
	}
}

/*
	指定したIDの兵士に攻撃を行わせる関数
	引数1: const std::string& id ID
	引数2: const Direction direction 攻撃方向
	返り値なし
*/
void CCommander::attack(const std::string& id, const Direction direction) const
{
	CInfantry* infantry = search_infantry_by_id(id);

	if (infantry != NULL) {
		infantry->attack(direction);
	}
}

/*
	指定したIDの兵士を移動する関数
	引数1: const std::string& id ID
	引数2: const Direction direction 移動方向
	返り値なし
*/
void CCommander::move(const std::string& id, const Direction direction) const
{
	CInfantry* infantry = search_infantry_by_id(id);

	if (infantry != NULL) {
		infantry->move(direction);
	}
}

/* private関数 */

/*
	全兵士のリストから，与えられたIDを持つ兵士を探す関数
	引数1: const std::string& id ID
	返り値: CInfantry* 与えられたIDを持つ兵士(存在しなければNULL)
*/
CInfantry* CCommander::search_infantry_by_id(const std::string& id) const
{
	for (auto infantry : m_infantries) {
		if (infantry->get_id() == id) {
			return infantry;
		}
	}

	return NULL;
}