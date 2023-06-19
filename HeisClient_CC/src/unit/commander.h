/**
*	@file		commander.h
*	@brief		heis 司令官クラス
*	@author		Ryo Konno
*	@details	チームの兵士を一元管理し、操作や状態取得を行う。
*/
#pragma once

#include "infantry.h"
#include "board_observer.h"
#include "board_controller.h"
#include "action_packet_builder.h"

#include <vector>

/**
*	@brief	司令官クラス
*	@details AIは、このクラスの各種メンバ関数を呼び出すことで、盤面上の兵士を操作できる。
*/
class CCommander
{
	// メンバ関数
public:
	// コンストラクタ
	explicit CCommander(std::string team_name, CBoard* board);

	// 指定した兵士の位置を取得する
	BoardPosition get_infantry_position_by_id(std::string id);
	// 指定した兵士のステータスを取得する
	InfantryStatus get_infantry_status_by_id(std::string id);

	// 兵士に攻撃を指示する
	void attack(std::string id, BoardPosition dst);
	// 兵士に移動を指示する
	void move(std::string id, BoardPosition dst);

	// 移動可能なすべてのマスを取得する
	std::vector<BoardPosition> find_movable_position(std::string id);
	// 攻撃可能なすべてのマスを取得する
	std::vector<BoardPosition> find_attackable_position(std::string id);

	// 行動可能な兵士のIDを取得する
	std::vector<std::string> get_all_actable_infantry_ids(std::string team_name);
	// 移動可能な兵士のIDを取得する
	std::vector<std::string> get_all_movable_infantry_ids(std::string team_name);
	// 攻撃可能な兵士のIDを取得する
	std::vector<std::string> get_all_attackable_infantry_ids(std::string team_name);

	// 「行動」パケットを作成する
	JSONSendPacket_Action create_action_pkt();

private:
	// IDから兵士を取得する
	InfantryWithPos find_infantry_by_id(std::string id);

	// メンバ変数
private:
	//! 自チーム名
	std::string m_team_name;

	//! 盤面
	CBoard* m_board;

	//! 盤面情報取得クラス
	CBoardObserver m_observer;
	//! 盤面操作クラス
	CBoardController m_controller;
	//! 「行動」パケット作成クラス
	CActionPacketBuilder m_builder;
};