// heis フィールドクラス
// Author: Ryo Konno
#include "field.h"

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
	備考: 座標は，最も左上のマスを(0, 0)とする
*/
CInfantry* CField::get_infantry(const uint16_t pos_x, const uint16_t pos_y) const
{
	// 指定された座標が不正でないかどうかチェック
	bool position_error = (pos_x < 0 || FieldParam_Width <= pos_x);
	position_error |= (pos_y < 0 || FieldParam_Height <= pos_y);
	if (position_error) {
		printf("指定された座標(%d, %d)は不正です\n", pos_x, pos_y);
		return NULL;
	}

	return m_field[pos_x + (FieldParam_Width * pos_y)];
}

/* private関数 */

/*
	フィールドを初期化する関数
	引数なし
	返り値なし
*/
void CField::initalize()
{
	m_field.fill(NULL);
}