/**
*	@file		board.h
*	@brief		heis 盤面クラス
*	@author		Ryo Konno
*	@details	heisの各種ユニットが配置される盤面の定義およびそれに対する操作を提供する．
*/
#pragma once

#include "infantry.h"
#include "JSON_data_packet.h"

#include <vector>


/**
*	@struct Square
*	@brief 盤面の各マス目
*/
struct Square
{
	/**
	*	@brief コンストラクタ
	*	@param[in] infantry 兵士のインスタンス
	*	@param[in] exists 兵士が実在するかどうか
	*/
	Square(const CInfantry& infantry, bool exists)
		: infantry(infantry)
		, exists(exists)
	{
		// Do Nothing
	}

	//! 兵士のインスタンス
	CInfantry infantry;
	//! 兵士が実在するかどうか
	bool exists;
};

/**
*	@struct BoardSize
*	@brief 盤面のサイズ
*/
struct BoardSize
{
	//! 幅
	size_t width;
	//! 高さ
	size_t height;
};

/**
*	@brief 盤面の各行を表す型
*/
typedef std::vector<Square> Row;
/**
*	@brief 盤面を表す型(2次元配列)
*/
typedef std::vector<Row> Grid;

/**
*	@brief	盤面クラス
*/
class CBoard
{
	/* 注) 座標は，最も左上のマスを(0, 0)とする */

	// メンバ関数
public:
	// コンストラクタ
	explicit CBoard(const JSONRecvPacket_Board& pkt);

	// デストラクタ
	~CBoard();

	// マス目を取得する
	Square get_square(const Coordinate2D& pos) const;
	// 兵士をセットする
	void set_infantry(const Coordinate2D& pos, const CInfantry& infantry);
	// 兵士を削除する
	void remove_infantry(const Coordinate2D& pos);

	// 盤面のサイズを取得する
	BoardSize get_size() const;

	// デバッグ，動作確認用
	void show() const;

	// メンバ変数
private:
	//! 盤面の実体
	Grid m_grid;
};