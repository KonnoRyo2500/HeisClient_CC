/**
*	@file		coodinate_2d.h
*	@brief		heis ���ʃf�[�^�\��
*	@author		Ryo Konno
*	@details	�v���O�������ŋ��ʂ��Ďg���\���̂�N���X���`����D
*/
#pragma once

#include <cstdint>
#include <cmath>

/**
*	@def INVALID_COODINATE
*	@brief �s���ȍ��W
*/
#define INVALID_COODINATE Coordinate2D(-1, -1)

/**
*	@struct Coordinate2D
*	@brief �Ֆʏ�̍��W
*/
struct Coordinate2D
{	
	// �����o�֐�
public:
	/**
	*	@brief �R���X�g���N�^
	*	@param[in] x x���W
	*	@param[in] y y���W
	*/
	explicit Coordinate2D(const int x,  const int y)
		: x(x)
		, y(y)
	{
		// Do Nothing
	}
	/**
	*	@brief �w�肳�ꂽ�_�Ƃ�L1�������v�Z����
	*	@param[in] dst L1���W���v�Z����_
	*	@return uint32_t ���g��dst�Ƃ�L1���W
	*/
	uint32_t calc_l1_distance(const Coordinate2D& dst) const
	{
		return std::abs(this->x - dst.x) + std::abs(this->y - dst.y);
	}
	/**
	*	@brief ��r���Z�q(==)
	*	@param[in] dst �E��
	*	@return bool ���Z�q�̉��Z����
	*/
	bool operator==(const Coordinate2D& dst) const
	{
		return x == dst.x && y == dst.y;
	}
	/**
	*	@brief ��r���Z�q(!=)
	*	@param[in] dst �E��
	*	@return bool ���Z�q�̉��Z����
	*/
	bool operator!=(const Coordinate2D& dst) const
	{
		return x != dst.x || y != dst.y;
	}

	// �����o�ϐ�
public:
	//! x���W
	int x;
	//! y���W
	int y;
};