// heis	パス生成クラス
// Author: Ryo Konno
#pragma once

#include <string>

// TODO: このクラスを共通ソースに移動させる
// このクラスはインスタンス化不要のため，staticクラスとする
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