/**
*	@file		commander.cpp
*	@brief		heis 司令官クラス
*	@author		Ryo Konno
*	@details	チームの兵士を一元管理し，操作や状態取得を行う．
*/

#include "commander.h"
#include "board.h"
#include "common.h"
#include <algorithm>

/* public関数 */

/**
*	@brief コンストラクタ
*	@param[in] team_name チーム名
*/
CCommander::CCommander(const std::string& team_name)
	: m_team_name(team_name)
{
	// 処理なし
}

/**
*	@brief デストラクタ
*/
CCommander::~CCommander()
{
	// 処理なし
}

/**
*	@brief 指定したIDを持つ兵士の位置を取得する関数
*	@param[in] id 兵士のID
*	@return BoardPosition 位置
*	@throws std::runtime_error 指定したIDの兵士がいない場合
*/
BoardPosition CCommander::get_position(const std::string& id) const
{
	CInfantry* infantry = search_infantry_by_id(id);

	if (infantry != NULL) {
		return infantry->get_position();
	}
	throw std::runtime_error("NULLの兵士の位置を取得しようとしています");
}

/**
*	@brief 指定したIDを持つ兵士の残り行動回数を取得する関数
*	@param[in] id 兵士のID
*	@return uint8_t 兵士の残り行動回数(指定したIDの兵士がいなければ-1)
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

/**
*	@brief 指定したIDを持つ兵士のHPを取得する関数
*	@param[in] id 兵士のID
*	@return int8_t 兵士のHP(指定したIDの兵士がいなければ-1)
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

/**
*	@brief 指定したIDの兵士に攻撃を行わせる関数
*	@param[in] id 兵士のID
*	@param[in] dst_pos 攻撃先の座標
*/
void CCommander::attack(const std::string& id, const BoardPosition dst_pos) const
{
	CInfantry* infantry = search_infantry_by_id(id);

	if (infantry != NULL && infantry->get_team_name() == m_team_name) {
		g_battle_log->write_log(CLog::LogLevel_InvisibleInfo, cc_common::format(
			"ID\"%s\"の兵士が，座標(%d, %d)に対して攻撃しました",
			id.c_str(), dst_pos.x, dst_pos.y));
		infantry->attack(dst_pos);
	}
}

/**
*	@brief 指定したIDの兵士を移動する関数
*	@param[in] id 兵士のID
*	@param[in] dst_pos 移動先の座標
*/
void CCommander::move(const std::string& id, const BoardPosition dst_pos) const
{
	CInfantry* infantry = search_infantry_by_id(id);

	if (infantry != NULL && infantry->get_team_name() == m_team_name) {
		g_battle_log->write_log(CLog::LogLevel_InvisibleInfo, cc_common::format(
			"ID\"%s\"の兵士が，座標(%d, %d)から(%d, %d)に移動しました",
			id.c_str(), infantry->get_position().x, infantry->get_position().y, dst_pos.x, dst_pos.y));
		infantry->move(dst_pos);
	}
}

/**
*	@brief 移動可能なすべてのマスを返す関数
*	@param[in] id 兵士のID
*	@return std::vector<BoardPosition> 移動可能なマス
*/
std::vector<BoardPosition> CCommander::find_movable_position(const std::string& id) const
{
	CInfantry* infantry = search_infantry_by_id(id);

	if (infantry != NULL) {
		return infantry->find_movable_position();
	}
	return std::vector<BoardPosition>();
}

/**
*	@brief 攻撃可能なすべてのマスを返す関数
*	@param[in] id 兵士のID
*	@return std::vector<BoardPosition> 攻撃可能なマス
*/
std::vector<BoardPosition> CCommander::find_attackable_position(const std::string& id) const
{
	CInfantry* infantry = search_infantry_by_id(id);

	if (infantry != NULL) {
		return infantry->find_attackable_position();
	}
	return std::vector<BoardPosition>();
}

/**
*	@brief 行動可能なすべての兵士のIDを取得する関数
*	@param[in] team_name 取得対象のチーム名
*	@return std::vector<std::string&> 行動可能な各兵士のID
*/
std::vector<std::string> CCommander::get_all_actable_infantry_ids(const std::string& team_name) const
{
	std::vector<std::string> actable_infantry_ids;

	for (CInfantry* infantry : m_infantries) {
		if (is_actable(infantry) && infantry->get_team_name() == team_name) {
			actable_infantry_ids.push_back(infantry->get_id());
		}
	}

	return actable_infantry_ids;
}

/**
*	@brief 移動可能なすべての兵士のIDを取得する関数
*	@param[in] team_name 取得対象のチーム名
*	@return std::vector<std::string&> 移動可能な各兵士のID
*/
std::vector<std::string> CCommander::get_all_movable_infantry_ids(const std::string& team_name) const
{
	std::vector<std::string> movable_infantry_ids;

	for (CInfantry* infantry : m_infantries) {
		if (is_movable(infantry) && infantry->get_team_name() == team_name) {
			movable_infantry_ids.push_back(infantry->get_id());
		}
	}

	return movable_infantry_ids;
}

/**
*	@brief 攻撃可能なすべての兵士のIDを取得する関数
*	@param[in] team_name 取得対象のチーム名
*	@return std::vector<std::string&> 攻撃可能な各兵士のID
*/
std::vector<std::string> CCommander::get_all_attackable_infantry_ids(const std::string& team_name) const
{
	std::vector<std::string> attackable_infantry_ids;

	for (CInfantry* infantry : m_infantries) {
		if (is_attackable(infantry) && infantry->get_team_name() == team_name) {
			attackable_infantry_ids.push_back(infantry->get_id());
		}
	}

	return attackable_infantry_ids;
}

/**
*	@brief 自チームの兵士IDをすべて表示する関数
*	@remark 動作確認・デバッグ用
*/
void CCommander::show_infantry_ids() const
{
	printf("%s: ", m_team_name.c_str());
	for (CInfantry* infantry : m_infantries) {
		printf("%s ", infantry->get_id().c_str());
	}
	printf("\n");
}

/**
*	@brief 「行動」パケットを作成する関数
*	@return JSONSendPacket_Action 「行動」パケット
*/
JSONSendPacket_Action CCommander::create_action_pkt() const
{
	JSONSendPacket_Action action_pkt;

	action_pkt.turn_team = m_team_name;
	for (CInfantry* infantry : m_infantries) {
		action_pkt.contents.push_back(infantry->create_contents_array_elem());
	}
	return action_pkt;
}

/**
*	@brief 盤面にいる兵士から，管理している兵士の情報を更新する関数
*/
void CCommander::update()
{
	// 古い兵士リストを全削除
	m_infantries.clear();

	// 盤面にいる兵士をすべて兵士リストに加える
	CBoard* board = CBoard::get_instance();
	for (int x = 0; x < board->get_width(); x++) {
		for (int y = 0; y < board->get_height(); y++) {
			CInfantry* infantry = board->get_infantry(BoardPosition(x, y));
			if (infantry != NULL) {
				m_infantries.push_back(infantry);
			}
		}
	}
}

/* private関数 */
/**
*	@brief 全兵士のリストから，与えられたIDを持つ兵士を探す関数
*	@param[in] id 兵士のID
*	@return CInfantry* 与えられたIDを持つ兵士(存在しなければNULL)
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

/**
*	@brief 与えられた兵士が何かしら行動可能かを調べる関数
*	@param[in] infantry 兵士
*	@return bool 兵士が行動可能か(true: 何かしらの行動ができる, false: 手詰まりになっている)
*/
bool CCommander::is_actable(const CInfantry* infantry) const
{
	return is_movable(infantry) || is_attackable(infantry);
}

/**
*	@brief 与えられた兵士に移動可能なマスがあるか調べる関数
*	@param[in] infantry 兵士
*	@return bool 兵士が移動可能か(true: 移動可能なマスがある, false: 移動可能なマスがない)
*/
bool CCommander::is_movable(const CInfantry* infantry) const
{
	if (infantry->get_action_remain() <= 0) {
		return false;
	}
	return infantry->find_movable_position().size() != 0;
}

/**
*	@brief 与えられた兵士が隣接したマスの兵士に攻撃可能かを調べる関数
*	@param[in] infantry 兵士
*	@return bool 兵士が攻撃可能か(true: 攻撃可能な兵士が隣のマスに存在する, false: 攻撃可能な兵士が隣のマスに存在しない)
*/
bool CCommander::is_attackable(const CInfantry* infantry) const
{
	if (infantry->get_action_remain() <= 0) {
		return false;
	}
	return infantry->find_attackable_position().size() != 0;
}