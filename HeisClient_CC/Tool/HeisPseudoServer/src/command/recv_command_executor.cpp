/**
*	@file		recv_command_executor.cpp
*	@brief		PS recvコマンド実行クラス
*	@author		Ryo Konno
*	@details	PSのrecvコマンドを実行する。
*/

#include "recv_command_executor.h"
#include "path.h"

#include <stdexcept>
#include <fstream>

/**
*	@brief コマンドを実行する
*	@param[in] command コマンド
*/
void CRecvCommandExecutor::execute(const std::vector<std::string>& command)
{
	if (command.size() <= 1) {
		throw std::runtime_error("recvコマンド: 最低1つのオプションが必要です");
		return;
	}

	std::string json = recv_json(command.back());
	output_json(command, json);
}

/**
*	@brief ソケットを登録する
*	@param[in] socket_first 先攻プレイヤー用ソケット
*	@param[in] socket_second 後攻プレイヤー用ソケット
*/
void CRecvCommandExecutor::set_sockets(const CSocket& socket_first, const CSocket& socket_second)
{
	m_socket_first = socket_first;
	m_socket_second = socket_second;
}

/**
*	@brief JSONを受信する
*	@param[in] from 受信元
*	@return std::string 受信したJSON
*/
std::string CRecvCommandExecutor::recv_json(const std::string& from)
{
	CSocket target_socket;

	// 受信元の決定
	if (from == "first") {
		target_socket = m_socket_first;
	}
	else if (from == "second") {
		target_socket = m_socket_second;
	}
	else {
		throw std::runtime_error("recvコマンド: 未定義の受信元です");
	}

	// JSONを受信
	std::string json = target_socket.recv();

	return json;
}

/**
*	@brief オプションに応じて、JSONを出力する
*	@param[in] command コマンド
*	@param[in] json JSON
*/
void CRecvCommandExecutor::output_json(const std::vector<std::string>& command, const std::string& json)
{
	std::string arg_to = command[1];

	if (arg_to == "file") {
		if (command.size() == 2) {
			throw std::runtime_error("recvコマンド: fileオプションにはファイル名が必要です");
		}
		output_json_to_file(command[2], json);
	}
	else if (arg_to == "print") {
		output_json_to_stdout(json);
	}
	else {
		throw std::runtime_error("recvコマンド: 未定義のオプションです");
	}
}

/**
*	@brief JSONをファイルに出力する
*	@param[in] filename 出力ファイル名
*	@param[in] json JSON
*	@remark JSONファイルは、setting/filesディレクトリから読み込まれる
*	@remark JSONは、1ファイルにつき1つ、1行で出力される想定
*/
void CRecvCommandExecutor::output_json_to_file(const std::string& filename, const std::string& json)
{
	// JSONファイルのパスを作成する
	std::string path = join({
		PS_SETTING_DIR,
		"files",
		filename
	});

	// JSONファイルを開く
	std::ofstream ofs(path);
	if (ofs.fail()) {
		throw std::runtime_error("recvコマンド: JSONファイルのオープンに失敗しました");
	}

	// JSONをファイルに書き込む
	ofs.write(json.c_str(), json.size());
}

/**
*	@brief JSONを標準出力に出力する
*	@param[in] json JSON
*/
void CRecvCommandExecutor::output_json_to_stdout(const std::string& json)
{
	printf("%s\n", json.c_str());
}
