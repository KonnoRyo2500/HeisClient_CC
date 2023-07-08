/**
*	@file		main.cpp
*	@brief		heis ���C������
*	@author		Ryo Konno
*	@details	���C���֐����`����t�@�C���D
*/

#include "game_local.h"
#include "game_online.h"
#include "audience_mode.h"
#include "log.h"

#include <map>
#include <iostream>

/**
*	@enum GameMode
*	�Q�[�����[�h
*/
enum GameMode {
	//! ���[�J�����[�h
	GameMode_Local,
	//! �I�����C�����[�h
	GameMode_Online,
	//! �ϐ탂�[�h
	GameMode_Audience,
};

/* static�֐� */
//! �Q�[�����[�h��I������
static GameMode ask_game_mode();
//! �Q�[�����J�n����
static void start_game(const GameMode mode);

/**
*	@brief ���C���֐�
*/
int main()
{
	try {
		// �e�탍�O�t�@�C�����쐬����
		CLog::start_logging();

		// �ΐ�����s
		CLog::write(CLog::LogLevel_Information, "CC�̎��s���J�n����܂���");
		start_game(ask_game_mode());
		CLog::write(CLog::LogLevel_Information, "CC�̎��s������Ɋ������܂���");

		// ���O�̋L�^���I��
		CLog::end_logging();
	}
	catch (const std::exception& e) {
		// ���O�̃C���X�^���X������ɍ쐬�ł��Ă��Ȃ��\�������邽�߁A�R���\�[���o�͂ɂ���
		fprintf(stderr, "CC�̎��s���ɃG���[���������܂���(���e: %s)\n", e.what());
		return EXIT_FAILURE;
	}

	return EXIT_SUCCESS;
}

/**
*	@brief ���[�U�ɃQ�[�����[�h��I��������֐�
*	@return GameMode �I�������Q�[�����[�h
*/
static GameMode ask_game_mode()
{
	// ���[�U���I�������l�ƃQ�[�����[�h�Ƃ̑Ή��\
	static const std::map<int, GameMode> selected_value_to_mode = {
		{1, GameMode_Local},
		{2, GameMode_Online},
		{3, GameMode_Audience},
	};
	auto it = selected_value_to_mode.end();

	while (it == selected_value_to_mode.end()) {
		// �Q�[�����[�h���m�肷��܂őI���𑱍s����
		try {
			std::string user_input_buf;
			printf("�Q�[�����[�h����͂��ĉ�����(1: ���[�J�����[�h, 2: �I�����C�����[�h, 3: �ϐ탂�[�h)\n");
			std::cin >> user_input_buf;
			it = selected_value_to_mode.find(stoi(user_input_buf));
		}
		catch (const std::exception& e) {
			printf("���͂��ꂽ�Q�[�����[�h�͎󗝂ł��܂���(���R: %s)\n", e.what());
			continue;
		}
	}

	return it->second;
}

/**
*	@brief �w�肵�����[�h�ŃQ�[�����J�n����֐�
*	@param[in] mode �Q�[�����[�h
*/
static void start_game(const GameMode mode)
{
	CGame* game;

	switch (mode) {
		case GameMode_Local:
			game = new CGameLocal();
			break;
		case GameMode_Online:
			game = new CGameOnline();
			break;
		case GameMode_Audience:
			game = new CAudienceMode();
			break;
		default:
			throw std::runtime_error("�Q�[�����[�h���s���ł�");
	}
	game->play_game();

	delete game;
	game = NULL;
}