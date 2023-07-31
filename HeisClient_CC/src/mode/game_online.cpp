/**
*	@file		game_online.cpp
*	@brief		heis ゲーム(オンラインモード)進行管理クラス
*	@author		Ryo Konno
*	@details	オンラインモードでheisのゲームを実行する．
*/
#include "game_online.h"
#include "board.h"
#include "setting_keys.h"
#include "ai_factory.h"
#include "board_json_converter.h"
#include "action_json_converter.h"
#include "result_json_converter.h"
#include "message_json_converter.h"
#include "name_json_converter.h"
#include "confirm_name_json_converter.h"
#include "online_setting_file.h"
#include "path.h"
#include "log.h"

/**
*	@def ONLINE_SETTING_FILE_NAME
*	@brief オンラインモード設定ファイルの名前
*/
#define ONLINE_SETTING_FILE_NAME "online_setting.csv"

/**
*	@brief heis ゲーム実行メイン処理
*/
void CGameOnline::play_game()
{
	CLog::write(CLog::LogLevel_Information, "オンラインモードでゲームを開始しました");

	// 設定ファイルの読み込み
	OnlineSetting setting = COnlineSettingFile().load(
		join({CC_SETTING_DIR, ONLINE_SETTING_FILE_NAME})
	);

	// 対戦の準備
	initialize_battle(setting);

	// 名前登録
	recv_name_request();
	name_entry(setting.team_name);
	name_register(setting);

	// 対戦
	BoardJsonConverter board_json_converter;
	ActionJsonConverter action_json_converter;
	ResultJsonConverter result_json_converter;
	CAIFactory factory = CAIFactory();
	bool is_my_team_won;
	while (true) {
		// サーバから受け取った「盤面」JSONから、「盤面」パケットを作成
		JSONRecvPacket_Board board_pkt = board_json_converter.from_json_to_packet(m_sck->recv('\n'));

		// 受信した「盤面」JSONの内容に基づいて、盤面を構成
		CBoard board(board_pkt);

		// 盤面を表示
		board.show();

		// 「盤面」パケットは一旦変数に持っておきたいため，while文の条件部で対戦終了の判定をしない
		if (board_pkt.finished.get_value()) {
			is_my_team_won = judge_win(board);
			break;
		}
		// 自分のターンでなければ，次の「盤面」JSON受信まで待つ
		if (board_pkt.turn_team.get_value() != m_team_name) {
			continue;
		}

		// AIと司令官インスタンスを作成
		CCommander commander = CCommander(m_team_name, &board);
		CAIBase* ai = factory.create_instance(commander, setting.ai_impl);

		// ユーザAIの行動
		ai->AI_main(board_pkt);

		// AIインスタンスを破棄
		delete ai;
		ai = NULL;

		// 「行動」パケットを作成して送信
		m_sck->send(action_json_converter.from_packet_to_json(commander.create_action_pkt()), '\n');

		// 「結果」パケットを受信
		JSONRecvPacket_Result result_pkt = result_json_converter.from_json_to_packet(m_sck->recv('\n'));
		// 「結果」パケットの内容を表示
		for (const auto& result_elem : result_pkt.result.get_value()) {
			CLog::write(
				CLog::LogLevel_Warning,
				CStringUtil::format(
					"サーバからエラーメッセージが送信されました(エラー内容: %s, 対象兵士ID: %s)",
					result_elem.error.get_value().c_str(),
					result_elem.unit_id.exists() ? result_elem.unit_id.get_value().c_str() : "なし"
				)
			);
		}
	}

	// 対戦終了
	finalize_battle();

	// 勝敗を表示
	CLog::write(CLog::LogLevel_Information, is_my_team_won ? "You win!" : "You lose...", true);
	CLog::write(CLog::LogLevel_Information, "ゲームが終了しました");
}

/**
*	@brief 対戦を開始する前の準備を行う関数
*	@param[in] setting オンラインモード設定値
*	@remark この関数では，名前確定前に生成できるインスタンスを生成する
*/
void CGameOnline::initialize_battle(const OnlineSetting& setting)
{
	m_sck = new CSocket();
	CLog::write(
		CLog::LogLevel_Information,
		CStringUtil::format(
			"サーバに接続します(IPアドレス: %s, ポート番号: %d)",
			setting.server_ip_addr.c_str(),
			setting.server_port_num
		)
	);

	// サーバに接続
	m_sck->connect(setting.server_ip_addr, setting.server_port_num);

	CLog::write(
		CLog::LogLevel_Information,
		CStringUtil::format(
			"サーバに接続しました(IPアドレス: %s, ポート番号: %d)",
			setting.server_ip_addr.c_str(),
			setting.server_port_num
		)
	);
}

/**
*	@brief サーバーから，名前要求を受信する関数
*/
void CGameOnline::recv_name_request() const
{
	// 受信したJSONは特に使用しないため、現状では破棄する実装とする
	m_sck->recv('\n');
}

/**
*	@brief 指定された名前をサーバーに送る関数
*	@param[in] name 名前
*/
void CGameOnline::name_entry(const std::string& name) const
{
	JSONSendPacket_Name name_pkt;
	name_pkt.team_name.set_value(name);
	m_sck->send(NameJsonConverter().from_packet_to_json(name_pkt), '\n');

	CLog::write(
		CLog::LogLevel_Information,
		CStringUtil::format(
			"チーム名をサーバに送信しました(チーム名: %s)",
			name.c_str()
		)
	);
}

/**
*	@brief サーバーから受信した名前をチーム名として登録する関数
*	@param[in] setting オンラインモード設定値
*/
void CGameOnline::name_register(const OnlineSetting& setting)
{
	std::string confirm_name_json = m_sck->recv('\n');
	JSONRecvPacket_ConfirmName confirm_name_pkt = ConfirmNameJsonConverter().from_json_to_packet(confirm_name_json);

	m_team_name = confirm_name_pkt.your_team.get_value();

	CLog::write(
		CLog::LogLevel_Information,
		CStringUtil::format(
			"チーム名が確定しました(チーム名: %s)",
			m_team_name.c_str()
		)
	);
}

/**
*	@brief 対戦終了後の後処理を行う関数
*/
void CGameOnline::finalize_battle()
{
	delete m_sck;

	m_sck = NULL;

	CLog::write(CLog::LogLevel_Information, "インスタンスの削除が完了しました");
}

/**
*	@brief 勝敗を判定する
*	@param[in] board 勝敗確定時の盤面
*	@return bool 自チームが勝っているか
*/
bool CGameOnline::judge_win(const CBoard& board) const
{
	BoardSize size = board.get_size();
	for (size_t y = 0; y < size.height; y++) {
		for (size_t x = 0; x < size.width; x++) {
			Square sq = board.get_square(Coordinate2D(x, y));
			if (!sq.exists) {
				continue;
			}

			// 自チームの兵士が一人でもいたら自チームの勝利
			return sq.infantry.get_status().team_name == m_team_name;
		}
	}

	return false;
}