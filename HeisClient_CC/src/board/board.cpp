/**
*	@file		board.cpp
*	@brief		heis 盤面クラス
*	@author		Ryo Konno
*	@details	heisの各種ユニットが配置される盤面の定義およびそれに対する操作を提供する．
*/
#include "board.h"
#include "common.h"
#include "log.h"

/**
*	@def EMPTY_SQUARE
*	@brief 空きマス
*/
#define EMPTY_SQUARE Square(CInfantry(InfantryStatus("", "")), false)

/**
*	@brief コンストラクタ
*	@param[in] pkt 「盤面」パケット
*/
CBoard::CBoard(const JSONRecvPacket_Board& pkt)
{
	// すべて空の盤面を作成する
	Row row(pkt.width.get_value(), EMPTY_SQUARE);
	m_grid = std::vector(pkt.height.get_value(), row);

	// 盤面に兵士を配置する
	std::vector<UnitsArrayElem> units = pkt.units.get_value();
	for (auto& unit : units) {
		BoardPosition infantry_pos = BoardPosition(
			unit.locate.get_value().x.get_value(),
			unit.locate.get_value().y.get_value()
		);
		InfantryStatus status = InfantryStatus(
			unit.team.get_value(),
			unit.unit_id.get_value(),
			unit.hp.get_value(),
			INFANTRY_ACTION_LIMIT
		);

		m_grid[infantry_pos.y][infantry_pos.x] = Square(CInfantry(status), true);
	}
}

/**
*	@brief デストラクタ
*/
CBoard::~CBoard()
{
	// Do Nothing
}

/**
*	@brief マス目を取得する
*	@param[in] pos 取得したいマス目の座標
*	@return Square マス目
*/
Square CBoard::get_square(const BoardPosition& pos) const
{
	return m_grid[pos.y][pos.x];
}

/**
*	@brief 兵士をセットする
*	@param[in] pos 兵士をセットする座標
*	@param[in] infantry セットする兵士
*/
void CBoard::set_infantry(const BoardPosition& pos, const CInfantry& infantry)
{
	m_grid[pos.y][pos.x] = Square(infantry, true);
}

/**
*	@brief 兵士を削除する
*	@param[in] pos 削除する兵士のいる座標
*	@remark 指定された座標に兵士がいない場合でもエラーにはしない
*/
void CBoard::remove_infantry(const BoardPosition& pos)
{
	if (!get_square(pos).exists) {
		return;
	}
	m_grid[pos.y][pos.x] = EMPTY_SQUARE;
}

/**
*	@brief 盤面のサイズを取得する
*	@return BoardSize 盤面のサイズ
*/
BoardSize CBoard::get_size() const
{
	BoardSize size;
	size.height = m_grid.size();
	size.width = m_grid[0].size();
	return size;
}

/**
*	@brief 盤面を画面に表示する関数
*/
void CBoard::show() const
{
	std::string show_str = "";
	BoardSize size = get_size();

	for (size_t y = 0; y < size.height; y++) {
		for (size_t x = 0; x < size.width; x++) {
			Square sq = get_square(BoardPosition(x, y));

			if (!sq.exists) {
				show_str += "----";
			}
			else {
				show_str += sq.infantry.get_status().id.c_str();
			}
			show_str += " ";
		}
		show_str += "\n";
	}
	show_str += "\n";

	// ログの先頭情報により，盤面が崩れて見えるのを防ぐため，先頭に改行を入れておく
	CLog::write(CLog::LogLevel_Information, "\n" + show_str, true);
}