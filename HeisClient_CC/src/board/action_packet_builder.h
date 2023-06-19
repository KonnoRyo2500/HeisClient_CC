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
	JSONSendPacket_Action build(CBoard board, std::string team_name);

	// 兵士が攻撃した座標を追加する
	void add_attack_destination(CInfantry infantry, BoardPosition atk_dst);

	// 内部状態を初期化する
	void clear();

private:
	// もし兵士が攻撃済みならば、攻撃先の座標を返す
	BoardPosition find_attack_destination_if_attacked(std::string id);

	// メンバ変数
private:
	//! 攻撃先の座標(IDと座標のペア)
	std::vector<std::pair<std::string, BoardPosition>> m_attack_destinations;
};