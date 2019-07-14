// heis フィールドクラス
// Author: Ryo Konno
#pragma once

#include "infantry.h"
#include "JSON_data_packet.h"
#include <vector>
#include <map>

// ユーザAIはこのクラスの関数を呼ばないこと
class CField
{
	// 構造体，列挙体など
	public:
		
	// メンバ関数
	public:
		// デストラクタ
		~CField();

		// フィールドの取得，作成，削除
		static CField* get_instance();
		static void create_field();
		static void delete_field();

		// マス目情報(各マスにいる兵士の情報)の管理
		CInfantry* get_infantry(const uint16_t pos_x, const uint16_t pos_y) const;
		void set_infantry(const uint16_t pos_x, const uint16_t pos_y, CInfantry* infantry);
		void remove_infantry(const uint16_t pos_x, const uint16_t pos_y);

		// フィールド情報の取得
		uint16_t get_width();
		uint16_t get_height();
	
		// フィールドの更新
		void update(const JSONRecvPacket_Field& field_pkt);

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
		void validate_position(const uint16_t pos_x, const uint16_t pos_y) const;
		void validate_size(const uint16_t width, const uint16_t height) const;

	// メンバ変数
	private:
		// マスが空の時に入る文字列
		static const std::string EMPTY_ID;

		// フィールドのインスタンス
		static CField* m_instance;

		// フィールドの各マス目(ユニットがいればそのID，いなければ空文字列)
		std::vector<std::string> m_grid;
		// フィールドにいる全兵士(findを使用し，ID->兵士の実体へのアクセスを高速化するため，コンテナとしてmapを用いる)
		std::map<std::string, CInfantry*> m_all_infantries;

		// フィールドのサイズ
		uint16_t m_width;
		uint16_t m_height;
};