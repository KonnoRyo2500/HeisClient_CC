﻿// heis 司令官クラス
// Author: Ryo Konno

#include "commander.h"
#include "field.h"
#include <algorithm>

/* public関数 */

/*
	コンストラクタ
	引数1: const std::string& team_name チーム名
	備考: こちらはオンラインモードの処理で呼ぶ
*/
CCommander::CCommander(const std::string& team_name)
	: m_team_name(team_name)
{
	// 処理なし
}

/*
	デストラクタ
	引数なし
*/
CCommander::~CCommander()
{
	// 処理なし
}

/*
	指定したIDを持つ兵士のx座標を取得する関数
	引数1: (const std::string& id ID
	返り値: uint16_t 兵士のx座標(指定したIDの兵士がいなければ-1)
*/
uint16_t CCommander::get_x_position(const std::string& id) const
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
	返り値: uint16_t 兵士のy座標(指定したIDの兵士がいなければ-1)
*/
uint16_t CCommander::get_y_position(const std::string& id) const
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
	返り値: uint8_t 兵士の残り行動回数(指定したIDの兵士がいなければ-1)
*/
uint8_t CCommander::get_action_remain(const std::string& id) const
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
	返り値: int8_t 兵士のHP(指定したIDの兵士がいなければ-1)
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

/*
	行動可能なすべての兵士のIDを取得する関数
	引数なし
	返り値: std::vector<std::string&> 行動可能な各兵士のID
*/
std::vector<std::string> CCommander::get_all_actable_infantry_ids() const
{
	std::vector<std::string> actable_infantry_ids;

	for (CInfantry* my_team_infantry : m_infantries) {
		if (is_actable(my_team_infantry)) {
			actable_infantry_ids.push_back(my_team_infantry->get_id());
		}
	}

	return actable_infantry_ids;
}

/*
	移動可能なすべての兵士のIDを取得する関数
	引数なし
	返り値: std::vector<std::string&> 移動可能な各兵士のID
*/
std::vector<std::string> CCommander::get_all_movable_infantry_ids() const
{
	std::vector<std::string> movable_infantry_ids;

	for (CInfantry* my_team_infantry : m_infantries) {
		if (is_movable(my_team_infantry)) {
			movable_infantry_ids.push_back(my_team_infantry->get_id());
		}
	}

	return movable_infantry_ids;
}

/*
	攻撃可能なすべての兵士のIDを取得する関数
	引数なし
	返り値: std::vector<std::string&> 攻撃可能な各兵士のID
*/
std::vector<std::string> CCommander::get_all_attackable_infantry_ids() const
{
	std::vector<std::string> attackable_infantry_ids;

	for (CInfantry* my_team_infantry : m_infantries) {
		if (is_attackable(my_team_infantry)) {
			attackable_infantry_ids.push_back(my_team_infantry->get_id());
		}
	}

	return attackable_infantry_ids;
}

/*
	自チームの兵士IDをすべて表示する関数
	引数なし
	返り値なし
	備考: 動作確認・デバッグ用
*/
void CCommander::show_infantry_ids()
{
	printf("%s: ", m_team_name.c_str());
	for (CInfantry* infantry : m_infantries) {
		printf("%s ", infantry->get_id().c_str());
	}
	printf("\n");
}

/*
	フィールドにいる兵士から，管理している兵士の情報を更新する関数
	引数なし
	返り値なし
*/
void CCommander::update()
{
	// 古い兵士リストを全削除
	m_infantries.clear();

	// フィールドにいる自チームの兵士をすべて兵士リストに加える
	CField* field = CField::get_instance();
	for (int x = 0; x < field->get_width(); x++) {
		for (int y = 0; y < field->get_height(); y++) {
			CInfantry* infantry = field->get_infantry(x, y);
			if (infantry != NULL && infantry->get_team_name() == m_team_name) {
				m_infantries.push_back(infantry);
			}
		}
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

/*
	与えられた兵士が何かしら行動可能かを調べる関数
	引数1: const CInfantry* infantry 兵士
	返り値: bool 兵士が行動可能か(true: 何かしらの行動ができる, false: 手詰まりになっている)
*/
bool CCommander::is_actable(const CInfantry* infantry) const
{
	return is_movable(infantry) || is_attackable(infantry);
}

/*
	与えられた兵士が隣接したマスに移動可能かを調べる関数
	引数1: const CInfantry* infantry 兵士
	返り値: bool 兵士が移動可能か(true: 移動可能なマスがある, false: 隣接したどのマスにも移動できない)
*/
bool CCommander::is_movable(const CInfantry* infantry) const
{
	if (infantry->get_action_remain() <= 0) {
		return false;
	}

	for (auto look_result : infantry->look_around()) {
		if (look_result.infantry == NULL) {
			return true;
		}
	}

	return false;
}

/*
	与えられた兵士が隣接したマスの兵士に攻撃可能かを調べる関数
	引数1: const CInfantry* infantry 兵士
	返り値: bool 兵士が攻撃可能か(true: 攻撃可能な兵士が隣のマスに存在する, false: 攻撃可能な兵士が隣のマスに存在しない)
*/
bool CCommander::is_attackable(const CInfantry* infantry) const
{
	if (infantry->get_action_remain() <= 0) {
		return false;
	}

	for (auto look_result : infantry->look_around()) {
		if ((look_result.infantry != NULL) && (look_result.infantry->get_team_name() != m_team_name)) {
			return true;
		}
	}

	return false;
}