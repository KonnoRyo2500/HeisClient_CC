/**
*	@file		path_generator.h
*	@brief		heis パス生成クラス
*	@author		Ryo Konno
*	@details	特定のファイルへのパスを生成する．
*/
#pragma once

#include <string>

// TODO: このクラスを共通ソースに移動させる
/**
*	@brief	ログ出力クラス
*	@remark このクラスはインスタンス化不要のため，staticクラスとする
*/
class CPathGenerator {
	// 構造体，列挙体など
	public:

	// メンバ関数
	public:
		// 実行ファイルのパスを取得
		static std::string get_exe_path();

	private:
		// コンストラクタ
		CPathGenerator();

	// メンバ変数
	private:
		// なし
};