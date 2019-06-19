// heis フィールドクラス
// Author: Ryo Konno
#pragma once

#include "infantry.h"
#include <array>

// ユーザAIはこのクラスの関数を呼ばないこと
class CField
{
	// 構造体，列挙体など
	public:
		/* JSON関連のデータ(JSONの仕様に関しては，資料「兵ズ通信仕様.ods」を参照すること) */
		// サーバから受け取るデータ
		// 「盤面」JSONの"units"配列に含まれる"locate"オブジェクトのデータ
		struct JSONRecvData_Locate {
			int16_t x;		// "x"(フィールド)
			int16_t y;		// "y"(フィールド)
		};

		// 「盤面」JSONに含まれる"units"配列の要素
		struct JSONRecvData_Unit {
			std::string type;				// "type"(フィールド)
			std::string unit_id;					// "unit_id"(フィールド)
			JSONRecvData_Locate locate;		// "locate"(オブジェクト)
			int8_t hp;						// "hp"(フィールド)
			std::string team;				// "team"(フィールド)
		};

		// 「盤面」JSONのデータ
		struct JSONRecvData_Field {
			int16_t width;							// "width"(フィールド)
			int16_t height;							// "height"(フィールド)
			std::string turn_team;					// "turn_team"(フィールド)
			std::vector<std::string> players;		// "players"(配列)
			bool finished;							// "finished"(フィールド)
			uint32_t count;							// "count"(フィールド)
			std::vector<JSONRecvData_Unit> units;	// "units"(配列)
		};

	// メンバ関数
	public:
		// フィールドの取得，作成，削除
		static CField* get_instance();
		static void create_field();
		static void delete_field();

		// マス目情報(各マスにいる兵士の情報)の管理
		CInfantry* get_infantry(const int16_t pos_x, const int16_t pos_y) const;
		void set_infantry(const int16_t pos_x, const int16_t pos_y, CInfantry* infantry);
	
		// 内部処理用(ユーザAIはこの処理を呼ばないこと)
		void update_field();

		// デバッグ，動作確認用
		void show();

	private:
		void initalize();
		void validate_position(const int16_t pos_x, const int16_t pos_y) const;

	// メンバ変数
	private:
		static CField* m_instance;												// インスタンス
		std::array<CInfantry*, FieldParam_Width * FieldParam_Height> m_grid;	// フィールドの各マス目
};