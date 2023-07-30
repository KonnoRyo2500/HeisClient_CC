/**
*	@file		send_command_executor.cpp
*	@brief		PS sendコマンド実行クラス
*	@author		Ryo Konno
*	@details	PSのsendコマンドを実行する。
*/

#include "send_command_executor.h"
#include "path.h"

#include <stdexcept>
#include <fstream>

/**
*	@brief コマンドを実行する
*	@param[in] command コマンド
*/
void CSendCommandExecutor::execute(const std::vector<std::string>& command)
{
	if (command.size() <= 1) {
		throw std::runtime_error("sendコマンド: 最低1つのオプションが必要です");
		return;
	}

	std::string json = load_json(command);
	send_json(command.back(), json);
}

/**
*	@brief ソケットを登録する
*	@param[in] socket_first 先攻プレイヤー用ソケット
*	@param[in] socket_second 後攻プレイヤー用ソケット
*/
void CSendCommandExecutor::set_sockets(const CSocket& socket_first, const CSocket& socket_second)
{
	m_socket_first = socket_first;
	m_socket_second = socket_second;
}

/**
*	@brief オプションに応じて、JSONを読み込む
*	@param[in] command コマンド
*	@return std::string 読み込んだJSON
*/
std::string CSendCommandExecutor::load_json(const std::vector<std::string>& command)
{
	std::string arg_from = command[1];
	std::string json;

	if (arg_from == "file") {
		if (command.size() == 2) {
			throw std::runtime_error("sendコマンド: fileオプションにはファイル名が必要です");
		}
		json = load_json_from_file(command[2]);
	}
	else {
		throw std::runtime_error("sendコマンド: 未定義のオプションです");
	}

	return json;
}

/**
*	@brief ファイルからJSONを読み込む
*	@param[in] filename ファイル名
*	@return std::string 読み込んだJSON
*	@remark JSONファイルは、setting/filesディレクトリから読み込まれる
*	@remark JSONは、1ファイルにつき1つ、1行で書かれている想定
*/
std::string CSendCommandExecutor::load_json_from_file(const std::string& filename)
{
	// JSONファイルのパスを作成する
	std::string path = join({
		PS_SETTING_DIR,
		"files",
		filename
	});

	// JSONファイルを開く
	std::ifstream ifs(path);
	if (ifs.fail()) {
		throw std::runtime_error("sendコマンド: JSONファイルのオープンに失敗しました");
	}

	// JSONを読み込む
	std::string json;
	std::getline(ifs, json);

	return json;
}

/**
*	@brief JSONを送信する
*	@param[in] command コマンド
*	@param[in] json 送信するJSON
*/
void CSendCommandExecutor::send_json(const std::string& to, const std::string& json)
{
	CSocket target_socket;

	// 送信先の決定
	if (to == "first") {
		target_socket = m_socket_first;
	}
	else if (to == "second") {
		target_socket = m_socket_second;
	}
	else {
		throw std::runtime_error("sendコマンド: 未定義の送信先です");
	}

	// JSONを送信
	target_socket.send(json);
}
