/**
*	@file		game_local.cpp
*	@brief		heis �Q�[��(���[�J�����[�h)�i�s�Ǘ��N���X
*	@author		Ryo Konno
*	@details	���[�J�����[�h��heis�̃Q�[�������s����D
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
*	@brief ���[�J�����[�h�ݒ�t�@�C���̖��O
*/
#define LOCAL_SETTING_FILE_NAME "local_setting.csv"

/* public�֐� */

/**
*	@brief ���[�J�����[�h���s���C������
*/
void CGameLocal::play_game()
{
	CLog::write(CLog::LogLevel_Information, "���[�J�����[�h�ŃQ�[�����J�n���܂���");

	// �ݒ�t�@�C���̓ǂݍ���
	LocalSetting setting = CLocalSettingFile().load(
		join({CC_SETTING_DIR, LOCAL_SETTING_FILE_NAME})
	);

	// �Ֆʂ̍쐬
	// ���[�J�����[�h�ł̓I�����C�����[�h�ƈقȂ�AJSON�ɍ��킹�ĔՖʂ��쐬���Ȃ�
	// ���̂��߁A�Ֆʂ͎����O�Ɉ�x�����쐬���Ă���
	JSONRecvPacket_Board board_pkt = create_initial_board_packet(setting);
	CBoard board(board_pkt);

	// �ΐ�
	std::string win_team_name = "";
	while (win_team_name == "") {
		// �Ֆʂ�\��
		board.show();

		if (board_pkt.finished.get_value()) {
			break;
		}

		// AI�C���X�^���X�𐶐�����
		std::string turn_team = board_pkt.turn_team.get_value();
		CCommander commander = CCommander(turn_team, &board);
		std::string ai_impl_name =
			(turn_team == setting.my_team_name ? setting.my_team_ai_impl : setting.enemy_team_ai_impl);
		CAIBase* ai = CAIFactory().create_instance(commander, ai_impl_name);

		// AI��1�^�[�����s��������
		ai->AI_main(board_pkt);

		// AI�C���X�^���X��j������
		delete ai;
		ai = NULL;

		// �^�[���I����̏���
		board_pkt.turn_team.set_value(get_next_turn_team_name(board_pkt, setting));
		win_team_name = get_winning_team_name(board);
		reset_infantry_action_remain(board);
	}

	// �Q�[���I�����̔Ֆʂ�\������
	board.show();

	// ���s��\�����ďI��
	CLog::write(CLog::LogLevel_Information, (win_team_name == setting.my_team_name ? "You win!" : "You lose..."), true);
	CLog::write(CLog::LogLevel_Information, "�Q�[�����I�����܂���");
}

/* private�֐� */

/**
*	@brief �ŏ��̃^�[���́u�Ֆʁv�p�P�b�g���쐬����
*	@param[in] setting ���[�J�����[�h�ݒ�l
*	@return JSONRecvPacket_Board �ŏ��̃^�[���́u�Ֆʁv�p�P�b�g
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
*	@brief �ŏ��́u�Ֆʁv�p�P�b�g��"units"�v�f���쐬����
*	@param[in] setting ���[�J�����[�h�ݒ�l
*	@return std::vector<UnitsArrayElem> �ŏ��́u�Ֆʁv�p�P�b�g��"units"�v�f
*/
std::vector<UnitsArrayElem> CGameLocal::create_units_of_initial_board_packet(const LocalSetting& setting) const
{
	std::vector<UnitsArrayElem> units;
	int serial_number = 1;

	// ���`�[���̕��m�����쐬
	for (auto& pos : setting.my_team_init_pos) {
		UnitsArrayElem elem;
		elem.team.set_value(setting.my_team_name);
		elem.type.set_value(INFANTRY_UNIT_TYPE);
		elem.hp.set_value(INFANTRY_INITIAL_HP);
		elem.unit_id.set_value(
			// �`�[�����̐擪2���� + �A��(0����2��)
			setting.my_team_name.substr(0, 2) + (serial_number <= 9 ? "0" : "") + std::to_string(serial_number)
		);

		LocateObjData locate;
		locate.x.set_value(pos.x);
		locate.y.set_value(pos.y);
		elem.locate.set_value(locate);

		units.push_back(elem);

		serial_number++;
	}

	// �G�`�[���̕��m�����쐬
	serial_number = 1;
	for (auto& pos : setting.enemy_team_init_pos) {
		UnitsArrayElem elem;
		elem.team.set_value(setting.enemy_team_name);
		elem.type.set_value(INFANTRY_UNIT_TYPE);
		elem.hp.set_value(INFANTRY_INITIAL_HP);
		elem.unit_id.set_value(
			// �`�[�����̐擪2���� + �A��(0����2��)
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
*	@brief �Ֆʏ�̕��m�̍s���񐔂����Z�b�g����
*	@param[out] board �Ֆ�
*/
void CGameLocal::reset_infantry_action_remain(CBoard& board) const
{
	BoardSize size = board.get_size();

	// �Ֆʏ�̑S���m��T�����A���̍s���񐔂����Z�b�g����
	for (int y = 0; y < size.height; y++) {
		for (int x = 0; x < size.width; x++) {
			Square sq = board.get_square(Coordinate2D(x, y));
			if (!sq.exists) {
				continue;
			}

			// ���m��������A�����X�e�[�^�X�ōs���񐔂݂̂����Z�b�g���ꂽ���m�ɒu��������
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
*	@brief ���̃^�[���̃`�[�������擾����
*	@param[in] pkt ���݂́u�Ֆʁv�p�P�b�g
*	@return std::string ���̃^�[���̃`�[����
*/
std::string CGameLocal::get_next_turn_team_name(const JSONRecvPacket_Board& pkt, const LocalSetting& setting) const
{
	return (pkt.turn_team.get_value() == setting.my_team_name ? setting.enemy_team_name : setting.my_team_name);
}

/**
*	@brief �������Ă���`�[�������擾����
*	@param[in] board ���݂̔Ֆ�
*	@return std::string �������Ă���`�[����(�ǂ�����������Ă��Ȃ��ꍇ�͋󕶎���)
*/
std::string CGameLocal::get_winning_team_name(const CBoard& board) const
{
	BoardSize size = board.get_size();
	std::vector<std::string> teams; // ������2�ȏ�̃`�[�������������玎���p����

	// �Ֆʏ�̑S���m��T�����A���̃`�[�������擾����
	for (int y = 0; y < size.height; y++) {
		for (int x = 0; x < size.width; x++) {
			Square sq = board.get_square(Coordinate2D(x, y));
			if (!sq.exists) {
				continue;
			}
			std::string team_name = sq.infantry.get_status().team_name;
			if (std::find(teams.begin(), teams.end(), team_name) == teams.end()) {
				// teams���̃`�[�����͏d�����Ȃ��悤�ɂ���
				teams.push_back(team_name);
			}
		}
	}

	// 1�̃`�[���̕��m�����c���Ă��Ȃ���΂��̃`�[���̏���
	return (teams.size() == 1 ? teams[0] : "");
}