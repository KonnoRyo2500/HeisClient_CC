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
		// プロジェクトのディレクトリを取得
		static std::string get_project_dir();

	private:
		// コンストラクタ
		CPathGenerator();

		// 実行ファイルのディレクトリを取得
		static std::string get_exe_dir();

	// メンバ変数
	private:
		// なし
};