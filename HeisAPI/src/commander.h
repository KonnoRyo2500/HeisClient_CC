// heis 司令官クラス
// Author: Ryo Konno
#pragma once

#include "infantry.h"
#include <vector>

#define INFANTRY_STATUS_ERROR (-1)				// 兵士のステータス取得エラー(指定したIDの兵士がチームの全兵士の中にいないなど)

class CCommander
{
	// 構造体，列挙体など
	public:

	// メンバ関数
	public:
		// コンストラクタ
		explicit CCommander(const std::string& team_name);
		explicit CCommander(const std::string& team_name, const int init_area_width, const int init_area_height, const bool is_bottom_left); // ローカルモード用

		// デストラクタ
		~CCommander();

		// 指定したIDを持つ兵士のステータス取得
		uint16_t get_x_position(const std::string& id) const;
		uint16_t get_y_position(const std::string& id) const;
		uint8_t get_action_remain(const std::string& id) const;
		int8_t get_hp(const std::string& id) const;

		// 行動(指定したIDを持つ兵士への命令)
		void attack(const std::string& id, const Direction direction) const;
		void move(const std::string& id, const Direction direction) const;

		// 兵士の探索
		std::vector<std::string> get_all_actable_infantry_ids() const;
		std::vector<std::string> get_all_movable_infantry_ids() const;
		std::vector<std::string> get_all_attackable_infantry_ids() const;

		// 内部処理用(ユーザAIはこの処理を呼ばないこと)
		void update_all_infantries();

		// デバッグ用
		void show_infantry_ids();

	private:
		// インスタンスの生成，削除時の処理
		void positioning_infantries(const int init_area_width, const int init_area_height, const bool is_bottom_left);
		void clear_infantries();
		std::string make_id(const std::string& prefix, const uint16_t number);

		// 兵士の追加，削除
		void append_infantry(const std::string& id, const uint16_t pos_x, const uint16_t pos_y);
		void remove_infantry(const std::string& id);

		// 兵士の探索
		CInfantry* search_infantry_by_id(const std::string& id) const;
		bool is_actable(const CInfantry* infantry) const;
		bool is_movable(const CInfantry* infantry) const;
		bool is_attackable(const CInfantry* infantry) const;

	// メンバ変数
	private:
		std::string m_team_name;					// チーム名
		std::vector<CInfantry*> m_infantries;		// チームの全兵士
};