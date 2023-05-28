/**
*	@file		sample_ai.h
*	@brief		heis �T���v��AI
*	@author		Ryo Konno
*	@details	AI�̃T���v�������B�����_���ɍs����I������B
*/

#pragma once

#include "ai_base.h"

/**
*	@brief �T���v��AI�N���X
*	@details �T���v��������AI�B�����_���ɍs����I�����܂��BAI�����̍ۂ̎Q�l�Ƃ��Ă��g�����������B
*/
class CSampleAI : public CAIBase
{
	// �\���́E�񋓑̂Ȃ�
private:
	/**
	*	@enum Action
	*	�s���̎��
	*/
	enum Action {
		//! �ړ�
		Action_Move,
		//! �U��
		Action_Attack,
	};

	// �����o�֐�
public:
	// �R���X�g���N�^
	explicit CSampleAI(CCommander* commander);

	// ���C������
	void AI_main(const JSONRecvPacket_Board& board_pkt);

private:
	// �����_���ȕ����Ɉړ�����
	void sample_random_move(const std::string infantry_id);
	// �����_���ȕ����ɍU������
	void sample_random_attack(const std::string infantry_id);
	// ���ɍs�����镺�m��I������
	std::string sample_select_next_infantry(const std::vector<std::string>& infantry_ids) const;
	// ���m���ǂ̍s�������邩�I������
	Action sample_decide_action() const;
};