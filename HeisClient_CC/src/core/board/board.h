/**
*	@file		board.h
*	@brief		heis �ՖʃN���X
*	@author		Ryo Konno
*	@details	heis�̊e�탆�j�b�g���z�u�����Ֆʂ̒�`����т���ɑ΂��鑀���񋟂���D
*/
#pragma once

#include "infantry.h"
#include "JSON_data_packet.h"

#include <vector>


/**
*	@struct Square
*	@brief �Ֆʂ̊e�}�X��
*/
struct Square
{
	/**
	*	@brief �R���X�g���N�^
	*	@param[in] infantry ���m�̃C���X�^���X
	*	@param[in] exists ���m�����݂��邩�ǂ���
	*/
	Square(const CInfantry& infantry, bool exists)
		: infantry(infantry)
		, exists(exists)
	{
		// Do Nothing
	}

	//! ���m�̃C���X�^���X
	CInfantry infantry;
	//! ���m�����݂��邩�ǂ���
	bool exists;
};

/**
*	@struct BoardSize
*	@brief �Ֆʂ̃T�C�Y
*/
struct BoardSize
{
	//! ��
	size_t width;
	//! ����
	size_t height;
};

/**
*	@brief �Ֆʂ̊e�s��\���^
*/
typedef std::vector<Square> Row;
/**
*	@brief �Ֆʂ�\���^(2�����z��)
*/
typedef std::vector<Row> Grid;

/**
*	@brief	�ՖʃN���X
*/
class CBoard
{
	/* ��) ���W�́C�ł�����̃}�X��(0, 0)�Ƃ��� */

	// �����o�֐�
public:
	// �R���X�g���N�^
	explicit CBoard(const JSONRecvPacket_Board& pkt);

	// �f�X�g���N�^
	~CBoard();

	// �}�X�ڂ��擾����
	Square get_square(const Coordinate2D& pos) const;
	// ���m���Z�b�g����
	void set_infantry(const Coordinate2D& pos, const CInfantry& infantry);
	// ���m���폜����
	void remove_infantry(const Coordinate2D& pos);

	// �Ֆʂ̃T�C�Y���擾����
	BoardSize get_size() const;

	// �f�o�b�O�C����m�F�p
	void show() const;

	// �����o�ϐ�
private:
	//! �Ֆʂ̎���
	Grid m_grid;
};