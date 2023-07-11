/**
*	@file		board.cpp
*	@brief		heis �ՖʃN���X
*	@author		Ryo Konno
*	@details	heis�̊e�탆�j�b�g���z�u�����Ֆʂ̒�`����т���ɑ΂��鑀���񋟂���D
*/
#include "board.h"
#include "common.h"
#include "log.h"

/**
*	@def EMPTY_SQUARE
*	@brief �󂫃}�X
*/
#define EMPTY_SQUARE Square(CInfantry(InfantryStatus("", "")), false)

/**
*	@brief �R���X�g���N�^
*	@param[in] pkt �u�Ֆʁv�p�P�b�g
*/
CBoard::CBoard(const JSONRecvPacket_Board& pkt)
{
	// ���ׂċ�̔Ֆʂ��쐬����
	Row row(pkt.width.get_value(), EMPTY_SQUARE);
	m_grid = std::vector(pkt.height.get_value(), row);

	// �Ֆʂɕ��m��z�u����
	std::vector<UnitsArrayElem> units = pkt.units.get_value();
	for (auto& unit : units) {
		BoardPosition infantry_pos = BoardPosition(
			unit.locate.get_value().x.get_value(),
			unit.locate.get_value().y.get_value()
		);
		InfantryStatus status = InfantryStatus(
			unit.team.get_value(),
			unit.unit_id.get_value(),
			unit.hp.get_value(),
			INFANTRY_ACTION_LIMIT
		);

		m_grid[infantry_pos.y][infantry_pos.x] = Square(CInfantry(status), true);
	}
}

/**
*	@brief �f�X�g���N�^
*/
CBoard::~CBoard()
{
	// Do Nothing
}

/**
*	@brief �}�X�ڂ��擾����
*	@param[in] pos �擾�������}�X�ڂ̍��W
*	@return Square �}�X��
*/
Square CBoard::get_square(const BoardPosition& pos) const
{
	return m_grid[pos.y][pos.x];
}

/**
*	@brief ���m���Z�b�g����
*	@param[in] pos ���m���Z�b�g������W
*	@param[in] infantry �Z�b�g���镺�m
*/
void CBoard::set_infantry(const BoardPosition& pos, const CInfantry& infantry)
{
	m_grid[pos.y][pos.x] = Square(infantry, true);
}

/**
*	@brief ���m���폜����
*	@param[in] pos �폜���镺�m�̂�����W
*	@remark �w�肳�ꂽ���W�ɕ��m�����Ȃ��ꍇ�ł��G���[�ɂ͂��Ȃ�
*/
void CBoard::remove_infantry(const BoardPosition& pos)
{
	if (!get_square(pos).exists) {
		return;
	}
	m_grid[pos.y][pos.x] = EMPTY_SQUARE;
}

/**
*	@brief �Ֆʂ̃T�C�Y���擾����
*	@return BoardSize �Ֆʂ̃T�C�Y
*/
BoardSize CBoard::get_size() const
{
	BoardSize size;
	size.height = m_grid.size();
	size.width = m_grid[0].size();
	return size;
}

/**
*	@brief �Ֆʂ���ʂɕ\������֐�
*/
void CBoard::show() const
{
	std::string show_str = "";
	BoardSize size = get_size();

	for (size_t y = 0; y < size.height; y++) {
		for (size_t x = 0; x < size.width; x++) {
			Square sq = get_square(BoardPosition(x, y));

			if (!sq.exists) {
				show_str += "----";
			}
			else {
				show_str += sq.infantry.get_status().id.c_str();
			}
			show_str += " ";
		}
		show_str += "\n";
	}
	show_str += "\n";

	// ���O�̐擪���ɂ��C�Ֆʂ�����Č�����̂�h�����߁C�擪�ɉ��s�����Ă���
	CLog::write(CLog::LogLevel_Information, "\n" + show_str, true);
}