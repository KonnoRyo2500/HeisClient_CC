/**
*	@file		game_online.cpp
*	@brief		heis �Q�[��(�I�����C�����[�h)�i�s�Ǘ��N���X
*	@author		Ryo Konno
*	@details	�I�����C�����[�h��heis�̃Q�[�������s����D
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
*	@brief �I�����C�����[�h�ݒ�t�@�C���̖��O
*/
#define ONLINE_SETTING_FILE_NAME "online_setting.csv"

/**
*	@brief heis �Q�[�����s���C������
*/
void CGameOnline::play_game()
{
	CLog::write(CLog::LogLevel_Information, "�I�����C�����[�h�ŃQ�[�����J�n���܂���");

	// �ݒ�t�@�C���̓ǂݍ���
	OnlineSetting setting = COnlineSettingFile().load(
		join({CC_SETTING_DIR, ONLINE_SETTING_FILE_NAME})
	);

	// �ΐ�̏���
	initialize_battle(setting);

	// ���O�o�^
	recv_name_request();
	name_entry(setting.team_name);
	name_register(setting);

	// �ΐ�
	BoardJsonConverter board_json_converter;
	ActionJsonConverter action_json_converter;
	ResultJsonConverter result_json_converter;
	CAIFactory factory = CAIFactory();
	bool is_my_team_won;
	while (true) {
		// �T�[�o����󂯎�����u�ՖʁvJSON����A�u�Ֆʁv�p�P�b�g���쐬
		JSONRecvPacket_Board board_pkt = board_json_converter.from_json_to_packet(m_sck->recv('\n'));

		// ��M�����u�ՖʁvJSON�̓��e�Ɋ�Â��āA�Ֆʂ��\��
		CBoard board(board_pkt);

		// �Ֆʂ�\��
		board.show();

		// �u�Ֆʁv�p�P�b�g�͈�U�ϐ��Ɏ����Ă����������߁Cwhile���̏������őΐ�I���̔�������Ȃ�
		if (board_pkt.finished.get_value()) {
			is_my_team_won = judge_win(board);
			break;
		}
		// �����̃^�[���łȂ���΁C���́u�ՖʁvJSON��M�܂ő҂�
		if (board_pkt.turn_team.get_value() != m_team_name) {
			continue;
		}

		// AI�Ǝi�ߊ��C���X�^���X���쐬
		CCommander commander = CCommander(m_team_name, &board);
		CAIBase* ai = factory.create_instance(commander, setting.ai_impl);

		// ���[�UAI�̍s��
		ai->AI_main(board_pkt);

		// AI�C���X�^���X��j��
		delete ai;
		ai = NULL;

		// �u�s���v�p�P�b�g���쐬���đ��M
		m_sck->send(action_json_converter.from_packet_to_json(commander.create_action_pkt()), '\n');

		// �u���ʁv�p�P�b�g����M
		JSONRecvPacket_Result result_pkt = result_json_converter.from_json_to_packet(m_sck->recv('\n'));
		// �u���ʁv�p�P�b�g�̓��e��\��
		for (const auto& result_elem : result_pkt.result.get_value()) {
			CLog::write(
				CLog::LogLevel_Warning,
				CStringUtil::format(
					"�T�[�o����G���[���b�Z�[�W�����M����܂���(�G���[���e: %s, �Ώە��mID: %s)",
					result_elem.error.get_value().c_str(),
					result_elem.unit_id.exists() ? result_elem.unit_id.get_value().c_str() : "�Ȃ�"
				)
			);
		}
	}

	// �ΐ�I��
	finalize_battle();

	// ���s��\��
	CLog::write(CLog::LogLevel_Information, is_my_team_won ? "You win!" : "You lose...", true);
	CLog::write(CLog::LogLevel_Information, "�Q�[�����I�����܂���");
}

/**
*	@brief �ΐ���J�n����O�̏������s���֐�
*	@param[in] setting �I�����C�����[�h�ݒ�l
*	@remark ���̊֐��ł́C���O�m��O�ɐ����ł���C���X�^���X�𐶐�����
*/
void CGameOnline::initialize_battle(const OnlineSetting& setting)
{
	m_sck = new CSocket();
	CLog::write(
		CLog::LogLevel_Information,
		CStringUtil::format(
			"�T�[�o�ɐڑ����܂�(IP�A�h���X: %s, �|�[�g�ԍ�: %d)",
			setting.server_ip_addr.c_str(),
			setting.server_port_num
		)
	);

	// �T�[�o�ɐڑ�
	m_sck->connect(setting.server_ip_addr, setting.server_port_num);

	CLog::write(
		CLog::LogLevel_Information,
		CStringUtil::format(
			"�T�[�o�ɐڑ����܂���(IP�A�h���X: %s, �|�[�g�ԍ�: %d)",
			setting.server_ip_addr.c_str(),
			setting.server_port_num
		)
	);
}

/**
*	@brief �T�[�o�[����C���O�v������M����֐�
*/
void CGameOnline::recv_name_request() const
{
	// ��M����JSON�͓��Ɏg�p���Ȃ����߁A����ł͔j����������Ƃ���
	m_sck->recv('\n');
}

/**
*	@brief �w�肳�ꂽ���O���T�[�o�[�ɑ���֐�
*	@param[in] name ���O
*/
void CGameOnline::name_entry(const std::string& name) const
{
	JSONSendPacket_Name name_pkt;
	name_pkt.team_name.set_value(name);
	m_sck->send(NameJsonConverter().from_packet_to_json(name_pkt), '\n');

	CLog::write(
		CLog::LogLevel_Information,
		CStringUtil::format(
			"�`�[�������T�[�o�ɑ��M���܂���(�`�[����: %s)",
			name.c_str()
		)
	);
}

/**
*	@brief �T�[�o�[�����M�������O���`�[�����Ƃ��ēo�^����֐�
*	@param[in] setting �I�����C�����[�h�ݒ�l
*/
void CGameOnline::name_register(const OnlineSetting& setting)
{
	std::string confirm_name_json = m_sck->recv('\n');
	JSONRecvPacket_ConfirmName confirm_name_pkt = ConfirmNameJsonConverter().from_json_to_packet(confirm_name_json);

	m_team_name = confirm_name_pkt.your_team.get_value();

	CLog::write(
		CLog::LogLevel_Information,
		CStringUtil::format(
			"�`�[�������m�肵�܂���(�`�[����: %s)",
			m_team_name.c_str()
		)
	);
}

/**
*	@brief �ΐ�I����̌㏈�����s���֐�
*/
void CGameOnline::finalize_battle()
{
	delete m_sck;

	m_sck = NULL;

	CLog::write(CLog::LogLevel_Information, "�C���X�^���X�̍폜���������܂���");
}

/**
*	@brief ���s�𔻒肷��
*	@param[in] board ���s�m�莞�̔Ֆ�
*	@return bool ���`�[���������Ă��邩
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

			// ���`�[���̕��m����l�ł������玩�`�[���̏���
			return sq.infantry.get_status().team_name == m_team_name;
		}
	}

	return false;
}