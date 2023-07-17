/**
*	@file		action_packet_builder.h
*	@brief		heis 「行動」パケット作成クラス
*	@author		Ryo Konno
*	@details	盤面上の兵士から「行動」パケットを作成する。
*/

#pragma once

#include "JSON_data_packet.h"
#include "board.h"

/**
*	@brief 「行動」パケット作成クラス
*/
class CActionPacketBuilder
{
	// メンバ関数
public:
	// 「行動」パケットを作成する
	JSONSendPacket_Action build(const CBoard& board, const std::string& team_name) const;

	// 兵士が攻撃した座標を追加する
	void add_attack_destination(const CInfantry& infantry, const Coordinate2D& atk_dst);

	// 内部状態を初期化する
	void clear();

private:
	// もし兵士が攻撃済みならば、攻撃先の座標を返す
	Coordinate2D find_attack_destination_if_attacked(const std::string& id) const;

	// メンバ変数
private:
	//! 攻撃先の座標(IDと座標のペア)
	std::vector<std::pair<std::string, Coordinate2D>> m_attack_destinations;
};