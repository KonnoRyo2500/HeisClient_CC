/**
*	@file		game_online.cpp
*	@brief		heis �Q�[��(�I�����C�����[�h)�i�s�Ǘ��N���X
*	@author		Ryo Konno
*	@details	�I�����C�����[�h��heis�̃Q�[�������s����D
*/
#include "game_online.h"
#include "const_val.h"
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

/* public�֐� */

/**
*	@brief heis �Q�[�����s���C������
*/
void CGameOnline::play_game()
{
	CLog::write(CLog::LogLevel_Information, "�I�����C�����[�h�ŃQ�[�����J�n���܂���");
	CLog::write(CLog::LogLevel_Information, "�I�����C�����[�h�ŃQ�[�����J�n���܂���");

	bool battle_result;

	// �ݒ�t�@�C���̓ǂݍ���
	OnlineSetting setting = COnlineSettingFile().load(
		join({CC_SETTING_DIR, ONLINE_SETTING_FILE_NAME})
	);

	// �ΐ�̏���
	initialize_battle(setting);

	recv_name_request();
	name_entry(setting.team_name);
	name_register(setting);

	// �ΐ�
	while (true) {
		// �T�[�o�����M����JSON���琶������p�P�b�g(�u�Ֆʁv�p�P�b�g�͂����Ő����ł���)
		BoardJsonConverter board_json_converter;
		ActionJsonConverter action_json_converter;
		ResultJsonConverter result_json_converter;
		JSONRecvPacket_Board board_pkt = board_json_converter.from_json_to_packet(m_sck->recv());
		JSONRecvPacket_Result result_pkt;

		// ��M�����u�ՖʁvJSON�̓��e�Ɋ�Â��āA�Ֆʂ��\��
		CBoard board(board_pkt);

		// �Ֆʂ�\��
		board.show();
		m_commander = new CCommander(m_team_name, &board);

		// �u�Ֆʁv�p�P�b�g�͈�U�ϐ��Ɏ����Ă����������߁Cwhile���̏������őΐ�I���̔�������Ȃ�
		if (board_pkt.finished.get_value()) {
			break;
		}
		// �����̃^�[���łȂ���΁C���́u�ՖʁvJSON��M�܂ő҂�
		if (board_pkt.turn_team.get_value() != m_team_name) {
			continue;
		}

		// ���[�UAI�̍s��
		m_ai->AI_main(board_pkt);

		// �u�s���v�p�P�b�g���쐬���đ��M
		m_sck->send(action_json_converter.from_packet_to_json(m_commander->create_action_pkt()));

		// �u���ʁv�p�P�b�g����M
		result_pkt = result_json_converter.from_json_to_packet(m_sck->recv());
		// �u���ʁv�p�P�b�g�̓��e��\��
		for (const auto& result_elem : result_pkt.result.get_value()) {
			CLog::write(CLog::LogLevel_Warning,cc_common::format(
				"�T�[�o����G���[���b�Z�[�W�����M����܂���(�G���[���e: %s, �Ώە��mID: %s)",
				result_elem.error.get_value().c_str(), 
				result_elem.unit_id.exists() ? result_elem.unit_id.get_value().c_str() : "�Ȃ�"));
		}
	}

	// �ΐ�I��
	battle_result = judge_win();
	finalize_battle();

	// ���s��\��
	CLog::write(CLog::LogLevel_Information, battle_result ? "You win!" : "You lose...", true);
	CLog::write(CLog::LogLevel_Information, "�Q�[�����I�����܂���");
}

/* private�֐� */

/**
*	@brief �ΐ���J�n����O�̏������s���֐�
*	@param[in] setting �I�����C�����[�h�ݒ�l
*	@remark ���̊֐��ł́C���O�m��O�ɐ����ł���C���X�^���X�𐶐�����
*/
void CGameOnline::initialize_battle(const OnlineSetting& setting)
{
	// m_commander, m_ai�̐����ɂ��ẮC���O�m���ɍs���K�v�����邽�߁Cname_register�֐��ōs��
	m_sck = new CSocket();
	CLog::write(CLog::LogLevel_Information, cc_common::format(
		"�T�[�o�ɐڑ����܂�(IP�A�h���X: %s, �|�[�g�ԍ�: %d)",
		setting.server_ip_addr.c_str(), setting.server_port_num));

	// �T�[�o�ɐڑ�
	m_sck->connect(setting.server_ip_addr, setting.server_port_num);

	CLog::write(CLog::LogLevel_Information, cc_common::format(
		"�T�[�o�ɐڑ����܂���(IP�A�h���X: %s, �|�[�g�ԍ�: %d)",
		setting.server_ip_addr.c_str(), setting.server_port_num));
	CLog::write(CLog::LogLevel_Information, "�C���X�^���X�̐������������܂���");
}

/**
*	@brief �T�[�o�[����C���O�v������M����֐�
*/
void CGameOnline::recv_name_request() const
{
	std::string received_JSON = m_sck->recv();
	MessageJsonConverter message_json_converter;
	JSONRecvPacket_Message name_req_msg_pkt = message_json_converter.from_json_to_packet(received_JSON);
}

/**
*	@brief �w�肳�ꂽ���O���T�[�o�[�ɑ���֐�
*	@param[in] name ���O
*/
void CGameOnline::name_entry(const std::string& name) const
{
	JSONSendPacket_Name name_pkt;
	NameJsonConverter name_json_converter;
	name_pkt.team_name.set_value(name);
	m_sck->send(name_json_converter.from_packet_to_json(name_pkt));
	CLog::write(CLog::LogLevel_Information, cc_common::format(
		"�`�[�������T�[�o�ɑ��M���܂���(�`�[����: %s)",
		name.c_str()));
}

/**
*	@brief �T�[�o�[�����M�������O���`�[�����Ƃ��ēo�^����֐�
*	@param[in] setting �I�����C�����[�h�ݒ�l
*/
void CGameOnline::name_register(const OnlineSetting& setting)
{
	std::string received_JSON = m_sck->recv();
	ConfirmNameJsonConverter confirm_name_json_converter;
	JSONRecvPacket_ConfirmName confirm_name_pkt = confirm_name_json_converter.from_json_to_packet(received_JSON);
	CAIFactory ai_factory = CAIFactory();

	m_team_name = confirm_name_pkt.your_team.get_value();
	m_ai = ai_factory.create_instance(
		*m_commander,
		setting.ai_impl
	);
	if (m_ai == NULL) {
		throw std::runtime_error("AI�C���X�^���X�����Ɏ��s���܂����BAI�����̐ݒ�����m�F��������");
	}

	CLog::write(CLog::LogLevel_Information, cc_common::format(
		"�`�[�������m�肵�܂���(�`�[����: %s)",
		m_team_name.c_str()));
}

/**
*	@brief �ΐ�I����̌㏈�����s���֐�
*/
void CGameOnline::finalize_battle()
{
	delete m_ai;
	delete m_sck;

	m_ai = NULL;
	m_sck = NULL;

	CLog::write(CLog::LogLevel_Information, "�C���X�^���X�̍폜���������܂���");
}

/**
*	@brief �ΐ�̌�����������C�Ֆʂ̏�Ԃ��珟�s�����肷��֐�
*	@return bool ���s(true: ���`�[���̏���, false: ���`�[���̕���)
*/
bool CGameOnline::judge_win() const
{
	// ���`�[���������Ă���΁C�G�̕��m�͂��Ȃ��̂ŁC���Ȃ��Ƃ�1�l�̕��m�͈ړ��ł���
	return m_commander->get_all_actable_infantry_ids(m_team_name).size() > 0;
}