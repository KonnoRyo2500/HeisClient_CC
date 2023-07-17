/**
*	@file		game_local.cpp
*	@brief		heis ゲーム(ローカルモード)進行管理クラス
*	@author		Ryo Konno
*	@details	ローカルモードでheisのゲームを実行する．
*/

#include "game_local.h"
#include "board.h"
#include "setting_keys.h"
#include "ai_factory.h"
#include "board_json_converter.h"
#include "local_setting_file.h"
#include "path.h"

/**
*	@def LOCAL_SETTING_FILE_NAME
*	@brief ローカルモード設定ファイルの名前
*/
#define LOCAL_SETTING_FILE_NAME "local_setting.csv"

/* public関数 */

/**
*	@brief ローカルモード実行メイン処理
*/
void CGameLocal::play_game()
{
	CLog::write(CLog::LogLevel_Information, "ローカルモードでゲームを開始しました");

	// 設定ファイルの読み込み
	LocalSetting setting = CLocalSettingFile().load(
		join({CC_SETTING_DIR, LOCAL_SETTING_FILE_NAME})
	);

	// 盤面の作成
	// ローカルモードではオンラインモードと異なり、JSONに合わせて盤面を作成しない
	// そのため、盤面は試合前に一度だけ作成しておく
	JSONRecvPacket_Board board_pkt = create_initial_board_packet(setting);
	CBoard board(board_pkt);

	// 対戦
	std::string win_team_name = "";
	while (win_team_name == "") {
		// 盤面を表示
		board.show();

		if (board_pkt.finished.get_value()) {
			break;
		}

		// AIインスタンスを生成する
		std::string turn_team = board_pkt.turn_team.get_value();
		CCommander commander = CCommander(turn_team, &board);
		std::string ai_impl_name =
			(turn_team == setting.my_team_name ? setting.my_team_ai_impl : setting.enemy_team_ai_impl);
		CAIBase* ai = CAIFactory().create_instance(commander, ai_impl_name);

		// AIを1ターン分行動させる
		ai->AI_main(board_pkt);

		// AIインスタンスを破棄する
		delete ai;
		ai = NULL;

		// ターン終了後の処理
		board_pkt.turn_team.set_value(get_next_turn_team_name(board_pkt, setting));
		win_team_name = get_winning_team_name(board);
		reset_infantry_action_remain(board);
	}

	// ゲーム終了時の盤面を表示する
	board.show();

	// 勝敗を表示して終了
	CLog::write(CLog::LogLevel_Information, (win_team_name == setting.my_team_name ? "You win!" : "You lose..."), true);
	CLog::write(CLog::LogLevel_Information, "ゲームが終了しました");
}

/* private関数 */

/**
*	@brief 最初のターンの「盤面」パケットを作成する
*	@param[in] setting ローカルモード設定値
*	@return JSONRecvPacket_Board 最初のターンの「盤面」パケット
*/
JSONRecvPacket_Board CGameLocal::create_initial_board_packet(const LocalSetting& setting) const
{
	JSONRecvPacket_Board pkt;

	pkt.count.set_value(0);
	pkt.finished.set_value(false);
	pkt.width.set_value(setting.board_width);
	pkt.height.set_value(setting.board_height);
	pkt.players.set_value(
		std::vector<std::string>{setting.my_team_name, setting.enemy_team_name}
	);
	pkt.turn_team.set_value(setting.first_turn_team);
	pkt.units.set_value(create_units_of_initial_board_packet(setting));

	return pkt;
}

/**
*	@brief 最初の「盤面」パケットの"units"要素を作成する
*	@param[in] setting ローカルモード設定値
*	@return std::vector<UnitsArrayElem> 最初の「盤面」パケットの"units"要素
*/
std::vector<UnitsArrayElem> CGameLocal::create_units_of_initial_board_packet(const LocalSetting& setting) const
{
	std::vector<UnitsArrayElem> units;
	int serial_number = 1;

	// 自チームの兵士情報を作成
	for (auto& pos : setting.my_team_init_pos) {
		UnitsArrayElem elem;
		elem.team.set_value(setting.my_team_name);
		elem.type.set_value(INFANTRY_UNIT_TYPE);
		elem.hp.set_value(INFANTRY_INITIAL_HP);
		elem.unit_id.set_value(
			// チーム名の先頭2文字 + 連番(0埋め2桁)
			setting.my_team_name.substr(0, 2) + (serial_number <= 9 ? "0" : "") + std::to_string(serial_number)
		);

		LocateObjData locate;
		locate.x.set_value(pos.x);
		locate.y.set_value(pos.y);
		elem.locate.set_value(locate);

		units.push_back(elem);

		serial_number++;
	}

	// 敵チームの兵士情報を作成
	serial_number = 1;
	for (auto& pos : setting.enemy_team_init_pos) {
		UnitsArrayElem elem;
		elem.team.set_value(setting.enemy_team_name);
		elem.type.set_value(INFANTRY_UNIT_TYPE);
		elem.hp.set_value(INFANTRY_INITIAL_HP);
		elem.unit_id.set_value(
			// チーム名の先頭2文字 + 連番(0埋め2桁)
			setting.enemy_team_name.substr(0, 2) + (serial_number <= 9 ? "0" : "") + std::to_string(serial_number)
		);

		LocateObjData locate;
		locate.x.set_value(pos.x);
		locate.y.set_value(pos.y);
		elem.locate.set_value(locate);

		units.push_back(elem);

		serial_number++;
	}

	return units;
}

/**
*	@brief 盤面上の兵士の行動回数をリセットする
*	@param[out] board 盤面
*/
void CGameLocal::reset_infantry_action_remain(CBoard& board) const
{
	BoardSize size = board.get_size();

	// 盤面上の全兵士を探索し、その行動回数をリセットする
	for (int y = 0; y < size.height; y++) {
		for (int x = 0; x < size.width; x++) {
			Square sq = board.get_square(Coordinate2D(x, y));
			if (!sq.exists) {
				continue;
			}

			// 兵士がいたら、同じステータスで行動回数のみがリセットされた兵士に置き換える
			CInfantry old_infantry = sq.infantry;
			InfantryStatus old_infantry_status = old_infantry.get_status();
			InfantryStatus new_infantry_status = InfantryStatus(
				old_infantry_status.team_name,
				old_infantry_status.id,
				old_infantry_status.hp,
				INFANTRY_ACTION_LIMIT
			);
			CInfantry new_infantry = CInfantry(new_infantry_status);

			board.set_infantry(Coordinate2D(x, y), new_infantry);
		}
	}
}

/**
*	@brief 次のターンのチーム名を取得する
*	@param[in] pkt 現在の「盤面」パケット
*	@return std::string 次のターンのチーム名
*/
std::string CGameLocal::get_next_turn_team_name(const JSONRecvPacket_Board& pkt, const LocalSetting& setting) const
{
	return (pkt.turn_team.get_value() == setting.my_team_name ? setting.enemy_team_name : setting.my_team_name);
}

/**
*	@brief 勝利しているチーム名を取得する
*	@param[in] board 現在の盤面
*	@return std::string 勝利しているチーム名(どちらも勝利していない場合は空文字列)
*/
std::string CGameLocal::get_winning_team_name(const CBoard& board) const
{
	BoardSize size = board.get_size();
	std::vector<std::string> teams; // ここに2つ以上のチーム名が入ったら試合継続中

	// 盤面上の全兵士を探索し、そのチーム名を取得する
	for (int y = 0; y < size.height; y++) {
		for (int x = 0; x < size.width; x++) {
			Square sq = board.get_square(Coordinate2D(x, y));
			if (!sq.exists) {
				continue;
			}
			std::string team_name = sq.infantry.get_status().team_name;
			if (std::find(teams.begin(), teams.end(), team_name) == teams.end()) {
				// teams内のチーム名は重複しないようにする
				teams.push_back(team_name);
			}
		}
	}

	// 1つのチームの兵士しか残っていなければそのチームの勝ち
	return (teams.size() == 1 ? teams[0] : "");
}