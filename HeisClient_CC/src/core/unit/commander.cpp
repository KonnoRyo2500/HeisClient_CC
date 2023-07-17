/**
*	@file		commander.cpp
*	@brief		heis 司令官クラス
*	@author		Ryo Konno
*	@details	チームの兵士を一元管理し、操作や状態取得を行う。
*/

#include "commander.h"

#include <iterator>
#include <algorithm>

/**
*	@brief コンストラクタ
*	@param[in] team_name チーム名
*	@param[in] board 盤面
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
*	@brief 指定したIDを持つ兵士の位置を取得する
*	@param[in] id 兵士ID
*	@return Coordinate2D 兵士の位置(指定したIDの兵士が見つからない場合はINVALID_POSITION)
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
*	@brief 指定した兵士のステータスを取得する
*	@param[in] id 兵士ID
*	@return InfantryStatus 兵士のステータス
*/
InfantryStatus CCommander::get_infantry_status_by_id(const std::string& id) const
{
	CInfantry infantry = find_infantry_by_id(id).first;
	return infantry.get_status();
}

/**
*	@brief 兵士に攻撃を指示する
*	@param[in] id 兵士のID
*	@param[in] dst 攻撃先の座標
*/
void CCommander::attack(const std::string& id, const Coordinate2D& dst)
{
	InfantryWithPos infantry_and_pos = find_infantry_by_id(id);
	CInfantry infantry = infantry_and_pos.first;
	Coordinate2D pos = infantry_and_pos.second;
	m_controller.attack(m_board, m_observer, pos, dst);

	// のちに「行動」パケットを作成できるようにするため、攻撃先を記録しておく
	m_builder.add_attack_destination(infantry, dst);
}

/**
*	@brief 兵士に移動を指示する
*	@param[in] id 兵士のID
*	@param[in] dst 移動先の座標
*/
void CCommander::move(const std::string& id, const Coordinate2D& dst) const
{
	InfantryWithPos infantry_and_pos = find_infantry_by_id(id);
	Coordinate2D pos = infantry_and_pos.second;
	m_controller.move(m_board, m_observer, pos, dst);
}

/**
*	@brief 移動可能なすべてのマスを取得する
*	@param[in] id 兵士のID
*	@return std::vector<Coordinate2D> 移動可能なマス
*/
std::vector<Coordinate2D> CCommander::find_movable_position(const std::string& id) const
{
	Coordinate2D pos = find_infantry_by_id(id).second;
	return m_observer.search_position_to_move(*m_board, pos);
}

/**
*	@brief 攻撃可能なすべてのマスを取得する
*	@param[in] id 兵士のID
*	@return std::vector<Coordinate2D> 攻撃可能なマス
*/
std::vector<Coordinate2D> CCommander::find_attackable_position(const std::string& id) const
{
	Coordinate2D pos = find_infantry_by_id(id).second;
	return m_observer.search_position_to_attack(*m_board, pos);
}

/**
*	@brief 行動可能な兵士のIDを取得する
*	@details 「行動可能」とは、移動もしくは攻撃ができるということを指す。
*	@param[in] team_name 兵士ID取得対象のチーム名
*	@return std::vector<std::string> 行動可能な兵士のID
*/
std::vector<std::string> CCommander::get_all_actable_infantry_ids(const std::string& team_name) const
{
	std::vector<std::string> movable_ids = get_all_movable_infantry_ids(team_name);
	std::vector<std::string> attackable_ids = get_all_attackable_infantry_ids(team_name);
	std::vector<std::string> actable_ids;

	// 移動可能、または攻撃可能な兵士のIDを探す
	std::copy(movable_ids.begin(), movable_ids.end(), std::back_inserter(actable_ids));
	for (auto& at_id : attackable_ids) {
		if (std::find(actable_ids.begin(), actable_ids.end(), at_id) != actable_ids.end()) {
			actable_ids.push_back(at_id);
		}
	}

	return actable_ids;
}

/**
*	@brief 移動可能な兵士のIDを取得する
*	@param[in] team_name 兵士ID取得対象のチーム名
*	@return std::vector<std::string> 移動可能な兵士のID
*/
std::vector<std::string> CCommander::get_all_movable_infantry_ids(const std::string& team_name) const
{
	std::vector<InfantryWithPos> infantries_and_pos = m_observer.fetch_all_infantry_and_position(*m_board);
	std::vector<std::string> ids;
	for (auto& ip : infantries_and_pos) {
		CInfantry infantry = ip.first;
		Coordinate2D pos = ip.second;

		// 指定されたチーム名で、かつ移動先の座標が存在する場合移動可能
		bool has_team_name = (infantry.get_status().team_name == team_name);
		bool has_pos_to_move = (m_observer.search_position_to_move(*m_board, pos).size() >= 1);
		if (has_team_name && has_pos_to_move) {
			ids.push_back(infantry.get_status().id);
		}
	}

	return ids;
}

/**
*	@brief 攻撃可能な兵士のIDを取得する
*	@param[in] team_name 兵士ID取得対象のチーム名
*	@return std::vector<std::string> 攻撃可能な兵士のID
*/
std::vector<std::string> CCommander::get_all_attackable_infantry_ids(const std::string& team_name) const
{
	std::vector<InfantryWithPos> infantries_and_pos = m_observer.fetch_all_infantry_and_position(*m_board);
	std::vector<std::string> ids;
	for (auto& ip : infantries_and_pos) {
		CInfantry infantry = ip.first;
		Coordinate2D pos = ip.second;

		// 指定されたチーム名で、かつ攻撃先の座標が存在する場合攻撃可能
		bool has_team_name = (infantry.get_status().team_name == team_name);
		bool has_pos_to_attack = (m_observer.search_position_to_attack(*m_board, pos).size() >= 1);
		if (has_team_name && has_pos_to_attack) {
			ids.push_back(infantry.get_status().id);
		}
	}

	return ids;
}

/**
*	@brief 「行動」パケットを作成する
*	@return JSONSendPacket_Action 「行動」パケット
*/
JSONSendPacket_Action CCommander::create_action_pkt()
{
	JSONSendPacket_Action action_pkt = m_builder.build(*m_board, m_team_name);
	m_builder.clear();
	return action_pkt;
}

/**
*	@brief IDから兵士と位置を取得する
*	@param[in] id 兵士ID
*	@return InfantryWithPos 兵士の実体とその位置
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