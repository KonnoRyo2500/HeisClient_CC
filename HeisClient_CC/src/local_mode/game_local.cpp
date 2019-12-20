// heis ゲーム(ローカルモード)進行管理クラス
// Author: Ryo Konno

#include "game_local.h"
#include "field.h"
#include "setting_keys.h"

#define LOCAL_SETTING_FILE_NAME "local_setting.csv"

/* public関数 */

/*
	heis ゲーム(ローカルモード)実行メイン処理
	引数なし
	返り値なし
*/
void CGameLocal::play_game()
{
	BattleResult battle_result;

	// 対戦開始前の準備
	initialize_battle();

	// 名前確定(ただし，名前は既に決まっているので，実際には何も行わない)
	recv_name_request();
	name_entry(LOCAL_MY_TEAM_NAME);
	name_register();

	// 対戦開始
	CurrentTurn current_turn = CurrentTurn_MyTurn;
	do {
		// ターン開始
		turn_entry();

		// ユーザAIの行動
		turn_action(current_turn);

		// ターン終了
		turn_end(current_turn);
	} while ((battle_result = judge_battle_result()) == BattleResult_Remain);

	// 対戦終了
	finalize_battle();

	// 勝敗を表示
	printf("%s\n", battle_result == BattleResult_MyTeamWin ? "You win!" : "You lose...");
}

/* private関数 */

/*
	ローカルモード設定ファイルから，設定を取得する関数
	引数なし
	返り値なし
*/
void CGameLocal::load_local_mode_setting()
{
	CCsvSettingFileReader local_setting_file(LOCAL_SETTING_FILE_NAME);

	m_setting.field_width = local_setting_file.get_single_value<uint16_t>(LOCAL_SETTING_KEY_FIELD_WIDTH, 0);
	m_setting.field_height = local_setting_file.get_single_value<uint16_t>(LOCAL_SETTING_KEY_FIELD_HEIGHT, 0);
	m_setting.my_team_name = local_setting_file.get_single_value<std::string>(LOCAL_SETTING_KEY_MY_NAME, 0);
	m_setting.enemy_team_name = local_setting_file.get_single_value<std::string>(LOCAL_SETTING_KEY_ENEMY_NAME, 0);
	m_setting.my_team_init_pos = get_initial_position(local_setting_file, LOCAL_SETTING_KEY_MY_INIT_POS);
	m_setting.enemy_team_init_pos = get_initial_position(local_setting_file, LOCAL_SETTING_KEY_ENEMY_INIT_POS);
	m_setting.is_my_team_first = 
		(local_setting_file.get_single_value<std::string>(LOCAL_SETTING_KEY_FIRST_TEAM, 0) == m_setting.my_team_name);
}

/*
	兵士の初期位置をローカルモード設定ファイルから取得する関数
	引数1: const CCsvSettingFileReader& local_setting_file ローカルモード設定ファイル
	引数2: const std::string& key 初期位置を取得するためのキー
	返り値: std::vector<FieldPosition> 初期位置
*/
std::vector<FieldPosition>  CGameLocal::get_initial_position(const CCsvSettingFileReader& local_setting_file, const std::string& key)
{
	// 初期位置は"x1 y1,x2 y2,..."の形式で記載されているものとする
	std::vector<std::string> all_init_pos_str = local_setting_file.get_all_value<std::string>(key);
	std::vector<FieldPosition> init_pos;

	for (auto& pos_str : all_init_pos_str) {
		CTokenManager tm;
		FieldPosition pos;
		token_array_t pos_token;

		pos_token = tm.split_string(pos_str, " ");
		if (pos_token.size() != 2) {
			throw CHeisClientException("兵士の初期位置の指定が不正です");
		}
		pos.x = stoi(tm.get_single_token(pos_token, 0));
		pos.y = stoi(tm.get_single_token(pos_token, 1));
		init_pos.push_back(pos);
	}

	return init_pos;
}

/*
	対戦を開始する前の準備を行う関数
	引数なし
	返り値なし
*/
void CGameLocal::initialize_battle()
{
	// 必要なインスタンスの生成
	CField::create_field();

	// 兵士の初期配置はここで行われる
	m_my_commander = new CCommander(LOCAL_MY_TEAM_NAME);
	m_enemy_commander = new CCommander(LOCAL_ENEMY_TEAM_NAME);

	m_my_AI = new CUserAI(m_my_commander);
	m_enemy_AI = new COpponentAI(m_enemy_commander);

	m_json_analyzer = new CJSONAnalyzer();

	m_pseudo_server = new CPseudoServer();
}

/*
	サーバーから，名前要求を受信する関数
	引数なし
	返り値なし
*/
void CGameLocal::recv_name_request() const
{
	JSONRecvPacket_Message msg_pkt = m_json_analyzer->create_message_pkt(m_pseudo_server->send_name_req_message_json());
}

/*
	指定された名前をサーバーに送る関数
	引数1: consbt std::string& name 名前
	返り値なし
*/
void CGameLocal::name_entry(const std::string name) const
{
	JSONSendPacket_Name name_pkt = { name };
	m_pseudo_server->recv_name_json(m_json_analyzer->create_name_JSON(name_pkt));
}

/*
	サーバーから受信した名前をチーム名として登録する関数
	引数なし
	返り値なし
*/
void CGameLocal::name_register() const
{
	// 名前の登録が必要ないため，処理なし
}

/*
	ターン開始時の処理を行う関数
	引数なし
	返り値なし
*/
void CGameLocal::turn_entry() const
{
	bool is_first_turn = (CField::get_instance()->get_width() == 0 || CField::get_instance()->get_height() == 0);
	JSONRecvPacket_Field field_pkt;

	// フィールドの更新
	// 最初のターンは，初期配置を記した「盤面」JSONを受け取る
	if (is_first_turn) {
		field_pkt = m_json_analyzer->create_field_pkt(m_pseudo_server->send_initial_field_json());
	}
	// それ以降のターンは，フィールドの状態をそのまま反映した「盤面」JSONを受け取る
	else {
		field_pkt = m_json_analyzer->create_field_pkt(m_pseudo_server->send_field_json());
	}
	CField::get_instance()->update(field_pkt);

	// 各司令官の持つ兵士情報を，最新のフィールド状態に合うよう更新
	m_my_commander->update();
	m_enemy_commander->update();
}

/*
	ユーザAI行動時の処理を行う関数
	引数1: const CurrentTurn current_turn 現在のターン
	返り値なし
*/
void CGameLocal::turn_action(const CurrentTurn current_turn) const
{
	if (current_turn == CurrentTurn_MyTurn) {
		m_my_AI->AI_main();
		m_pseudo_server->recv_action_json(m_json_analyzer->create_action_JSON(m_my_commander->create_action_pkt()));
	}
	else {
		m_enemy_AI->AI_main();
		m_pseudo_server->recv_action_json(m_json_analyzer->create_action_JSON(m_enemy_commander->create_action_pkt()));
	}
}

/*
	ターン終了時の処理を行う関数
	参照1: CurrentTurn& current_turn 現在のターン
	返り値なし
	備考: ターンを切り替える処理があるため，現在ターンは引数ではなく参照となる
*/
void CGameLocal::turn_end(CurrentTurn& current_turn) const
{
	// フィールドの表示
	CField::get_instance()->show();

	// ターンの切り替え
	if (current_turn == CurrentTurn_MyTurn) {
		current_turn = CurrentTurn_EnemyTurn;
	}
	else {
		current_turn = CurrentTurn_MyTurn;
	}
}

/*
	対戦終了後の後処理を行う関数
	引数なし
	返り値なし
*/
void CGameLocal::finalize_battle()
{
	delete m_my_commander;
	delete m_enemy_commander;
	delete m_my_AI;
	delete m_enemy_AI;
	delete m_json_analyzer;
	delete m_pseudo_server;

	m_my_commander = NULL;
	m_enemy_commander = NULL;
	m_my_AI = NULL;
	m_enemy_AI = NULL;
	m_json_analyzer = NULL;
	m_pseudo_server = NULL;

	CField::delete_field();
}

/*
	対戦結果を判定する関数
	引数なし
	返り値: BattleResult 対戦結果
*/
CGameLocal::BattleResult CGameLocal::judge_battle_result() const
{
	CField* field = CField::get_instance();
	bool my_team_alive = false;
	bool enemy_team_alive = false;

	// フィールド上のすべての兵士を探索し，少なくとも一方のチームの兵士が全滅していたら終了
	for (int x = 0; x < LocalFieldSize_Width; x++) {
		for (int y = 0; y < LocalFieldSize_Height; y++) {
			CInfantry* infantry = field->get_infantry(FieldPosition(x, y));

			if (infantry != NULL) {
				if (infantry->get_team_name() == LOCAL_MY_TEAM_NAME) {
					my_team_alive = true;
				}
				else if (infantry->get_team_name() == LOCAL_ENEMY_TEAM_NAME) {
					enemy_team_alive = true;
				}
			}
		}
	}

	BattleResult result = BattleResult_Remain;

	if (!my_team_alive) {
		result = BattleResult_EnemyTeamWin;
	}
	else if (!enemy_team_alive) {
		result = BattleResult_MyTeamWin;
	}

	return result;
}