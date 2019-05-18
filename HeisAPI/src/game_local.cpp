// heis ゲーム(ローカルモード)進行管理クラス
// Author: Ryo Konno

#include "game_local.h"
#include "field.h"

/*
	heis ゲーム(ローカルモード)実行メイン処理
	引数なし
	返り値なし
*/
void CGameLocal::play_game()
{
	bool battle_result;

	// 対戦開始前の準備
	prepare_to_battle();

	// 対戦
	while (!is_battle_end()) {
		// ユーザAIの行動
	}

	// 勝敗判定
	battle_result = judge_win();
	// 勝敗を表示
	printf("%s\n", battle_result ? "You win!" : "You lose...");
}

/*
	対戦を開始する前の準備を行う関数
	引数なし
	返り値なし
*/
void CGameLocal::prepare_to_battle()
{
	CField::create_field();
	// 兵士の初期配置
	positioning_infantry();
}

/*
	兵士をフィールド上に初期配置する関数
	引数なし
	返り値なし
*/
void CGameLocal::positioning_infantry()
{
	const bool start_from_bottom_left = true;			// 自軍の初期配置を左下にするか(falseだと右上になる)
	const int infantry_positioning_area_width = 6;		// 兵士を初期配置する領域(四角形)の幅
	const int infantry_positioning_area_height = 4;		// 兵士を初期配置する領域(四角形)の高さ

	// 兵士の初期配置
	CField* field = CField::get_instance();

	if (start_from_bottom_left) {
		for (int x = 0; x < infantry_positioning_area_width; x++) {
			for (int y = 0; y < infantry_positioning_area_height; y++) {

			}
		}
	}
	else {

	}
}

/*
	兵士のIDを作成する関数
	引数1: const std::string& prefix 接頭辞(IDの先頭につく2文字のアルファベット)
	引数2: const int16_t number IDの数字部分
	返り値: std::string 作成されたID
*/
std::string CGameLocal::make_id(const std::string& prefix, const int16_t number)
{
	return "te01";
}

/*
	対戦が終了したか判定する関数
	引数なし
	返り値: bool 対戦が終了しているか
*/
bool CGameLocal::is_battle_end()
{
	return true;
}

/*
	対戦の決着がついた後，フィールドの状態から勝敗を決定する関数
	引数なし
	返り値: bool 勝敗(true: 自チームの勝ち, false: 自チームの負け)
*/
bool CGameLocal::judge_win()
{
	return true;
}