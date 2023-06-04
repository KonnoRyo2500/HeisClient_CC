/**
*	@file		board.h
*	@brief		heis 盤面クラス
*	@author		Ryo Konno
*	@details	heisの各種ユニットが配置される盤面の定義およびそれに対する操作を提供する．
*/
#pragma once

#include "infantry.h"
#include "JSON_data_packet.h"
#include "log.h"
#include <vector>
#include <map>

/**
*	@brief	盤面クラス
*/
class CBoard
{
	// 構造体，列挙体など
	public:
		
	// メンバ関数
	public:
		// デストラクタ
		~CBoard();

		// 盤面の取得，作成，削除
		static CBoard* get_instance();
		static void create_board();
		static void delete_board();

		// マス目情報(各マスにいる兵士の情報)の管理
		CInfantry* get_infantry(const BoardPosition& pos) const;
		void set_infantry(const BoardPosition& pos, CInfantry* infantry);
		void remove_infantry(const BoardPosition& pos);

		// 盤面情報の取得
		uint16_t get_width();
		uint16_t get_height();
	
		// 盤面の更新
		void update(const JSONRecvPacket_Board& board_pkt);

		// デバッグ，動作確認用
		void show();

	private:
		// 初期化
		void initalize();

		// マス目の操作
		void create_grid(const uint16_t width, const uint16_t height);
		void clear_grid();

		// 兵士リスト中の兵士の操作
		CInfantry* find_infantry_by_id(const std::string& infantry_id) const;
		void delete_all_infantries();
		void add_infantry(CInfantry* new_infantry);
		void relocate_all_infantries_from_units_array(const std::vector<UnitsArrayElem>& units_array);

		// 値の検証
		void validate_position(const BoardPosition& pos) const;
		void validate_size(const uint16_t width, const uint16_t height) const;

	// メンバ変数
	private:
		//! マスが空の時に入る文字列
		static const std::string EMPTY_ID;

		//! 盤面のインスタンス
		static CBoard* m_instance;

		//! 盤面の各マス目(ユニットがいればそのID，いなければ空文字列)
		std::vector<std::string> m_grid;
		//! 盤面にいる全兵士(findを使用し，ID->兵士の実体へのアクセスを高速化するため，コンテナとしてmapを用いる)
		std::map<std::string, CInfantry*> m_all_infantries;

		//! 盤面のサイズ
		uint16_t m_width;
		uint16_t m_height;
};