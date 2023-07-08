/**
*	@file		board_observer.cpp
*	@brief		heis 盤面情報取得クラス
*	@author		Ryo Konno
*	@details	盤面上の兵士に関する各種情報を取得する。
*/

#include "board_observer.h"

#include <assert.h>
#include <algorithm>

/**
*	@brief 盤面上の全兵士と位置を取得する
*	@param[in] board 盤面
*	@return std::vector<InfantryWithPos> 盤面上の全兵士とその位置
*/
std::vector<InfantryWithPos> CBoardObserver::fetch_all_infantry_and_position(const CBoard& board) const
{
	std::vector<InfantryWithPos> infantries_and_positions;
	BoardSize size = board.get_size();

	for (size_t y = 0; y < size.height; y++) {
		for (size_t x = 0; x < size.width; x++) {
			Square sq = board.get_square(BoardPosition(x, y));
			if (!sq.exists) {
				continue;
			}
			
			BoardPosition pos = BoardPosition(x, y);
			infantries_and_positions.push_back(std::make_pair(sq.infantry, pos));
		}
	}

	return infantries_and_positions;
}

/**
*	@brief 指定された位置にいる兵士が移動できるマスを取得する
*	@param[in] board 盤面
*	@param[in] pos 取得対象の兵士の位置
*	@return std::vector<BoardPosition> 移動できるマス(指定したIDの兵士がいない場合は常に空)
*/
std::vector<BoardPosition> CBoardObserver::search_position_to_move(const CBoard& board, const BoardPosition& pos) const
{
	// posからL1距離が2以内のマスを得るための差分
	std::vector<int> dxs{ 0, -1, 0, 1, -2, -1, 1, 2, -1, 0, 1, 0 };
	std::vector<int> dys{ 2, 1, 1, 1, 0, 0, 0, 0, -1, -1, -1, -2 };
	assert(dxs.size() == dys.size());

	// 移動可能なマスを追加して返す
	std::vector<BoardPosition> pos_to_move;
	for (size_t i = 0; i < dxs.size(); i++) {
		BoardPosition dst = BoardPosition(pos.x + dxs[i], pos.y + dys[i]);
		if (can_move(board, pos, dst)) {
			pos_to_move.push_back(dst);
		}
	}

	return pos_to_move;
}

/**
*	@brief 指定された兵士が攻撃できるマスを取得する
*	@param[in] board 盤面
*	@param[in] pos 取得対象の兵士の位置
*	@return std::vector<BoardPosition> 攻撃できるマス(指定したIDの兵士がいない場合は常に空)
*/
std::vector<BoardPosition> CBoardObserver::search_position_to_attack(const CBoard& board, const BoardPosition& pos) const
{
	// posの上下左右のマスを得るための差分
	std::vector<int> dxs{ 0, 0, -1, 1 };
	std::vector<int> dys{ -1, 1, 0, 0 };
	assert(dxs.size() == dys.size());

	std::vector<BoardPosition> pos_to_attack;
	Square src_sq = board.get_square(pos);
	for (size_t i = 0; i < dxs.size(); i++) {
		BoardPosition dst = BoardPosition(pos.x + dxs[i], pos.y + dys[i]);
		if (can_attack(board, pos, dst)) {
			pos_to_attack.push_back(dst);
		}
	}

	return pos_to_attack;
}

/**
*	@brief 指定された座標が盤面の範囲内にあるか判定する
*	@param[in] board 盤面
*	@param[in] coord 座標
*	@return coordがboard内にあるか
*/
bool CBoardObserver::is_coord_in_board(const CBoard& board, const BoardPosition& coord) const
{
	BoardSize size = board.get_size();
	return
		(coord.x >= 0 && coord.x < size.width) &&
		(coord.y >= 0 && coord.y < size.height);
}

/**
*	@brief 指定されたマスに移動できるかどうかを判定する
*	@param[in] board 盤面
*	@param[in] src 移動元のマス
*	@param[in] dst 移動先のマス
*	@return bool 移動できるかどうか
*/
bool CBoardObserver::can_move(const CBoard& board, const BoardPosition& src, const BoardPosition& dst) const
{
	// 1: dstが盤面上に存在すること
	if (!is_coord_in_board(board, dst)) {
		return false;
	}

	// 2: dstに兵士がいないこと
	Square dst_sq = board.get_square(dst);
	if (dst_sq.exists) {
		return false;
	}

	// 3: srcに兵士がいること
	Square src_sq = board.get_square(src);
	if (!src_sq.exists) {
		return false;
	}

	// 4: 兵士の移動回数が足りていること
	uint8_t action_remain = src_sq.infantry.get_status().action_remain;
	if (action_remain < src.calc_l1_distance(dst)) {
		return false;
	}

	// 5: srcからdstに移動するためのパスが存在すること
	if (!exists_path(board, src, dst)) {
		return false;
	}

	// 上記1?5をすべて満たすとき、移動可能
	return true;
}

/**
*	@brief 指定されたマスに移動するためのパスが存在するかどうか判定する
*	@details srcを起点に盤面上のマスを上下左右にたどっていき、dstにたどり着ければパスが存在する。ただし、途中に敵がいる場合は先に進めず、味方は通り抜け可能
*	@param[in] board 盤面
*	@param[in] infantry 兵士の実体
*	@param[in] src 移動元のマス
*	@param[in] dst 移動先のマス
*	@return bool srcからdstに至るパスが存在するかどうか
*/
bool CBoardObserver::exists_path(const CBoard& board, const BoardPosition& src, const BoardPosition& dst) const
{
	Square src_sq = board.get_square(src);
	Square dst_sq = board.get_square(dst);
	// srcに兵士がいない
	if (!src_sq.exists) {
		return false;
	}
	// dstにすでに兵士がいる
	if (dst_sq.exists) {
		return false;
	}

	std::vector<BoardPosition> searching_coords{ src };
	std::vector<BoardPosition> searched_coords{ src };

	// dstに到達するまで、パスを探索
	int max_iteration = src_sq.infantry.get_status().action_remain;
	for (int i = 0; i < max_iteration; i++) {
		std::vector<BoardPosition> next_searching_coords;
		for (auto& sc : searching_coords) {
			// scの上下左右を探索
			std::vector<BoardPosition> around_coords{
				BoardPosition(sc.x, sc.y - 1),
				BoardPosition(sc.x, sc.y + 1),
				BoardPosition(sc.x - 1, sc.y),
				BoardPosition(sc.x + 1, sc.y)
			};

			for (auto& ac : around_coords) {
				// acはすでに探索済み
				if (std::find(searched_coords.begin(), searched_coords.end(), ac) != searched_coords.end()) {
					continue;
				}
				// acが盤面の範囲外
				if (!is_coord_in_board(board, ac)) {
					continue;
				}
				// dstに至るパスが見つかった
				if (ac == dst) {
					return true;
				}
				// acが空きマス、もしくは味方のいるマスであれば次回のsearching_coordsとする
				Square ac_sq = board.get_square(ac);
				if (!ac_sq.exists || ac_sq.infantry.get_status().team_name == src_sq.infantry.get_status().team_name) {
					next_searching_coords.push_back(ac);
				}
				
				// acを探索済みマスに加える
				searched_coords.push_back(ac);
			}
		}

		// 探索したマスを起点に、次回のパス探索を行う
		searching_coords = next_searching_coords;
	}

	// dstに至るパスが見つからなかった
	return false;
}

/**
*	@brief 指定されたマスに攻撃できるかどうか判定する
*	@param[in] board 盤面
*	@param[in] src 移動元のマス
*	@param[in] dst 移動先のマス
*	@return bool 攻撃できるかどうか
*/
bool CBoardObserver::can_attack(const CBoard& board, const BoardPosition& src, const BoardPosition& dst) const
{
	// 1: dstが盤面上に存在すること
	if (!is_coord_in_board(board, dst)) {
		return false;
	}

	// 2: dstに兵士がいること
	Square dst_sq = board.get_square(dst);
	if (!dst_sq.exists) {
		return false;
	}

	// 3: srcに兵士がいること
	Square src_sq = board.get_square(src);
	if (!src_sq.exists) {
		return false;
	}

	// 4: 兵士の移動回数が足りていること
	uint8_t action_remain = src_sq.infantry.get_status().action_remain;
	if (action_remain == 0) {
		return false;
	}

	// 5: srcの兵士と、dstの兵士のチーム名が違うこと
	std::string src_team = src_sq.infantry.get_status().team_name;
	std::string dst_team = dst_sq.infantry.get_status().team_name;
	if (src_team == dst_team) {
		return false;
	}

	// 上記1?5をすべて満たすとき、攻撃可能
	return true;
}