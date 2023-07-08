/**
*	@file		JSON_data_packet.h
*	@brief		heis JSON�f�[�^�Ǘ��p�p�P�b�g
*	@author		Ryo Konno
*	@details	CC�ƃT�[�o�Ԃł���肳���JSON���瓾���������舵�����߂̍\����(�p�P�b�g)���`����D
*/
#pragma once

#include <string>
#include <vector>
#include <stdexcept>

/**
*	@struct JsonElement
*	@brief JSON�̗v�f
*/
template <typename T>
struct JsonElement
{
// �����o�֐�
public:
	// �l���Z�b�g����
	void set_value(T value) {
		this->value = value;
		exists_value = true;
	}
	// �l���擾����
	T get_value() const {
		return value;
	}
	// �l�����݂��邩
	bool exists() const {
		return exists_value;
	}
	
// �����o�ϐ�
private:
	T value;
	bool exists_value = false;
};

/* �ȉ��̃p�P�b�g�̍\���́C�ʐM�d�l���Œ�`����Ă���JSON�̎d�l�Ɋ�Â� */

/* �e��p�P�b�g�̗v�f */

/**
*	@struct	ContentsArrayElem
*	@brief �u�s���v�p�P�b�g -> contents(�z��)�̗v�f
*/
struct ContentsArrayElem {
	//! "unit_id"�t�B�[���h�̒l
	JsonElement<std::string> unit_id;
	//! "to"."x"�t�B�[���h�̒l
	JsonElement<uint16_t> to_x;
	//! "to"."y"�t�B�[���h�̒l
	JsonElement<uint16_t> to_y;
	//! "atk"."x"�t�B�[���h�̒l
	JsonElement<uint16_t> atk_x;
	//! "atk"."y"�t�B�[���h�̒l
	JsonElement<uint16_t> atk_y;
};
 
/**
*	@struct LocateObjData
*	@brief �u�Ֆʁv�p�P�b�g -> units -> locate(�I�u�W�F�N�g)�̃f�[�^
*/
struct LocateObjData {
	//! "x"(�t�B�[���h)
	JsonElement<uint16_t> x;
	//! "y"(�t�B�[���h)
	JsonElement<uint16_t> y;
};

/**
*	@struct UnitsArrayElem
*	@brief �u�Ֆʁv�p�P�b�g -> units�̗v�f
*/
struct UnitsArrayElem {
	//! "type"(�t�B�[���h)
	JsonElement<std::string> type;
	//! "unit_id"(�t�B�[���h)
	JsonElement<std::string> unit_id;
	//! "locate"(�I�u�W�F�N�g)
	JsonElement<LocateObjData> locate;
	//! "hp"(�t�B�[���h)
	JsonElement<int8_t> hp;
	//! "team"(�t�B�[���h)
	JsonElement<std::string> team;
};

/**
*	@struct ResultArrayElem
*	@brief �u���ʁv�p�P�b�g -> result�̗v�f
*/
struct ResultArrayElem {
	//! "unit_id"(�t�B�[���h, �ȗ��\)
	JsonElement<std::string> unit_id;
	//! "error"(�t�B�[���h)
	JsonElement<std::string> error;
};

/* �T�[�o�ɑ��M����JSON���쐬���邽�߂̃p�P�b�g */
 
/**
*	@struct JSONSendPacket_Name
*	@brief �u���O�v�p�P�b�g
*	@details �u���O�vJSON�̏����i�[����p�P�b�g
*/
struct JSONSendPacket_Name {
	//! "team_name"(�t�B�[���h)
	JsonElement<std::string> team_name;
};

/**
*	@struct JSONSendPacket_Action
*	@brief �u�s���v�p�P�b�g
*	@details �u�s���vJSON�̏����i�[����p�P�b�g
*/
struct JSONSendPacket_Action {
	//! "turn_team"(�t�B�[���h)
	JsonElement<std::string> turn_team;
	//! "contents"(�z��)
	JsonElement<std::vector<ContentsArrayElem>> contents;
};

/* �T�[�o�����M����JSON�̃f�[�^���i�[����p�P�b�g */

// �u�Ֆʁv�p�P�b�g
/**
*	@struct JSONRecvPacket_Board
*	@brief �u�Ֆʁv�p�P�b�g
*	@details �u�ՖʁvJSON�̏����i�[����p�P�b�g
*/
struct JSONRecvPacket_Board {
	//! "width"(�t�B�[���h)
	JsonElement<uint16_t> width;
	//! "height"(�t�B�[���h)
	JsonElement<uint16_t> height;
	//! "turn_team"(�t�B�[���h)
	JsonElement<std::string> turn_team;
	//! "players"(�z��)
	JsonElement<std::vector<std::string>> players;
	//! "finished"(�t�B�[���h)
	JsonElement<bool> finished;
	//! "count"(�t�B�[���h)
	JsonElement<uint32_t> count;
	//! "units"(�z��)
	JsonElement<std::vector<UnitsArrayElem>> units;
};

/**
*	@struct JSONRecvPacket_ConfirmName
*	@brief �u���O�m��v�p�P�b�g
*	@details �u���O�m��vJSON�̏����i�[����p�P�b�g
*/
struct JSONRecvPacket_ConfirmName {
	//! "your_team"(�t�B�[���h)
	JsonElement<std::string> your_team;
};

/**
*	@struct JSONRecvPacket_Result
*	@brief �u���ʁv�p�P�b�g
*	@details �u���ʁvJSON�̏����i�[����p�P�b�g
*/
struct JSONRecvPacket_Result {
	//! "result"(�z��)
	JsonElement<std::vector<ResultArrayElem>> result;
};

/**
*	@struct JSONRecvPacket_Message
*	@brief �u���b�Z�[�W�v�p�P�b�g
*	@details �u���b�Z�[�W�vJSON�̏����i�[����p�P�b�g
*/
struct JSONRecvPacket_Message {
	//! "message"(�t�B�[���h)
	JsonElement<std::string> message;
};