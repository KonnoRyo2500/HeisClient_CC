﻿// heis フィールドクラス
// Author: Ryo Konno
#include "field.h"
#include "heis_client_exception.h"

CField* CField::m_instance = NULL;

/* public関数 */

/*
	フィールドのインスタンスを取得する関数
	引数なし
	返り値: CField* フィールドのインスタンス(へのポインタ)
*/
CField* CField::get_instance()
{
	return m_instance;
}

/*
	フィールドを作成する関数
	引数なし
	返り値なし
*/
void CField::create_field()
{
	if (m_instance == NULL) {
		m_instance = new CField;
		m_instance->initalize();
	}
}

/*
	フィールドを削除する関数
	引数なし
	返り値なし
*/
void CField::delete_field()
{
	delete m_instance;
	m_instance = NULL;
}

/*
	指定された座標にいる兵士を取得する関数
	引数1: const uint16_t pos_x 取得したい兵士のいるx座標
	引数2: const uint16_t pos_y 取得したい兵士のいるy座標
	返り値: CInfantry* 指定された座標にいる兵士(存在しなければNULL)
	例外: 指定した座標が範囲外のとき
	備考: 座標は，最も左上のマスを(0, 0)とする
*/
CInfantry* CField::get_infantry(const uint16_t pos_x, const uint16_t pos_y) const
{
	// 指定された座標が不正でないかどうかチェック
	validate_position(pos_x, pos_y);

	return m_grid[pos_x + (FieldParam_Width * pos_y)];
}

/*
	指定した座標に兵士を配置する関数
	引数1: const uint16_t pos_x 兵士を配置するx座標
	引数2: const uint16_t pos_y 兵士を配置するy座標
	引数3: CInfantry* infantry 配置する兵士
	返り値なし
	例外: 指定した座標が範囲外のとき
	備考: 座標は，最も左上のマスを(0, 0)とする
*/
void CField::set_infantry(const uint16_t pos_x, const uint16_t pos_y, CInfantry* infantry)
{
	// 指定された座標が不正でないかどうかチェック
	validate_position(pos_x, pos_y);

	m_grid[pos_x + (FieldParam_Width * pos_y)] = infantry;
}

/*
	フィールドの状態を更新する関数
	引数なし
	返り値なし
*/
void CField::update()
{
	
}

/*
	フィールドを画面に表示する関数
	引数なし
	返り値なし
	備考: 動作確認・デバッグ用
*/
void CField::show()
{
	for (int y = 0; y < FieldParam_Height; y++) {
		for (int x = 0; x < FieldParam_Width; x++) {
			CInfantry* infantry = get_infantry(x, y);

			if (infantry == NULL) {
				printf("----");
			}
			else {
				printf("%s", infantry->get_id().c_str());
			}
			printf(" ");
		}
		printf("\n");
	}
	printf("\n");
}

/* private関数 */

/*
	フィールドを初期化する関数
	引数なし
	返り値なし
*/
void CField::initalize()
{
	m_grid.fill(NULL);
}

/*
	座標を検証する関数
	引数1: const uint16_t pos_x x座標
	引数2: const uint16_t pos_y y座標
	返り値なし
	例外: 指定した座標が範囲外のとき
*/
void CField::validate_position(const uint16_t pos_x, const uint16_t pos_y) const
{
	// if文で条件判定すると条件文が長くなるのでこのような書き方をした
	bool is_position_valid = (0 <= pos_x && pos_x < FieldParam_Width);

	is_position_valid &= (0 <= pos_y && pos_y < FieldParam_Height);
	if (!is_position_valid) {
		throw CHeisClientException("指定された座標(%d, %d)は不正です", pos_x, pos_y);
	}
}