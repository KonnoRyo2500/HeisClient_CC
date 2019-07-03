// heis JSON解析クラス
// Author: Ryo Konno
#pragma once

#include "JSON_data_packet.h"
#include "picojson.h"
#include "heis_client_exception.h"

class CJSONAnalyzer{
	// 構造体，列挙体など
	public:
		// 解析したJSONの種別
		enum AnalyzedJSONKind {
			AnalyzedJSONKind_Field,				// 「盤面」JSON
			AnalyzedJSONKind_Result,			// 「結果」JSON
			AnalyzedJSONKind_NameDecided,		// 「名前確定」JSON
			AnalyzedJSONKind_Message,			// 「メッセージ」JSON
			AnalyzedJSONKind_UnknownJSON,		// 未定義のJSON
			AnalyzedJSONKind_NoJSONAnalyzed,	// まだJSONを解析していない
		};

	private:

	// メンバ関数
	public:
		// コンストラクタ
		explicit CJSONAnalyzer();

		// デストラクタ
		~CJSONAnalyzer();

		// 各種パケットをJSONに変換
		std::string create_action_JSON(const JSONSendPacket_Action& action_pkt) const;
		std::string create_name_JSON(const JSONSendPacket_Name& name_pkt) const;

		// 解析したJSONから各種パケットを生成
		JSONRecvPacket_NameDecided create_name_decided_pkt(const std::string& name_decided_JSON);
		JSONRecvPacket_Result create_result_pkt(const std::string& result_JSON);
		JSONRecvPacket_Message create_message_pkt(const std::string& message_JSON);
		JSONRecvPacket_Field create_field_pkt(const std::string& field_JSON);

		// 解析したJSONの種別を取得
		AnalyzedJSONKind get_analyzed_JSON_kind();

	private:
		// JSONを解析
		void analyze_JSON(const std::string& src_JSON);

		// JSON作成補助
		std::string serialize_JSON_obj(const picojson::object& obj) const;
		picojson::array make_contents_JSON(const std::vector<ContentsArrayElem>& contents_data) const;

		// 各種パケット作成補助
		std::vector<ResultArrayElem> make_result_array(const picojson::array& result_array) const;
		std::vector<std::string> make_players_array(const picojson::array& players_array) const;
		std::vector<UnitsArrayElem> make_units_array(const picojson::array& units_array) const;
		LocateObjData make_locate_object(const picojson::object& locate_obj) const;

		// 解析したJSONの種類を判定
		AnalyzedJSONKind distinguish_analyzed_JSON_kind() const;

		// JSON種類判定処理
		AnalyzedJSONKind check_whether_name_decided_JSON() const;
		AnalyzedJSONKind check_whether_result_JSON() const;
		AnalyzedJSONKind check_whether_message_JSON() const;
		AnalyzedJSONKind check_whether_field_JSON() const;

		// JSON種類判定補助
		bool exists_key_in_JSON_object(const std::string& key, const picojson::object& obj) const;

		// 解析したJSONと要求するJSONの種類が一致しているか
		void validate_JSON_kind(const AnalyzedJSONKind req_JSON_kind) const;

		// テンプレート関数
		/*
			直近に解析したJSONから，キーを指定して必須でない数値を取得する関数
			引数1: const std::string& key キー名
			引数2: const picojson::object& src_JSON_obj 値の取得元となるJSONオブジェクト
			返り値: T 取得した値
			例外: 指定されたキーに対する必須の値が存在しないとき
		*/
		template <typename T>
		OptionalVal<T> get_optional_number_val(const std::string& key, const picojson::object& src_JSON_obj) const
		{
			OptionalVal<T> ret_optional_val;

			if (src_JSON_obj.find(key) != src_JSON_obj.end()) {
				// picojsonの仕様上，数値型はdouble型でしか取り扱えないので，一旦double型で取得する
				// std::mapの[]演算子は非const関数のため，at関数を用いる
				ret_optional_val.set_val(static_cast<T>(src_JSON_obj.at(key).get<double>()));
				ret_optional_val.clear_omit_flag();
			}
			else {
				// キーに対応する値がなければ，デフォルト値を返す
				// デフォルト値の生成には，std::mapの[]演算子で得られるデフォルト値を返す
				std::map<std::string, T> dummy_map;

				ret_optional_val.set_val(dummy_map[key]);
				ret_optional_val.set_omit_flag();
			}

			return ret_optional_val;
		}

		/*
			直近に解析したJSONから，キーを指定して必須でない非数値(文字列型の値，真偽値型の値, オブジェクトなど)を取得する関数
			引数1: const std::string& key キー名
			引数2: const picojson::object& src_JSON_obj 値の取得元となるJSONオブジェクト
			返り値: T 取得した値
			例外: 指定されたキーに対する必須の値が存在しないとき
		*/
		template <typename T>
		OptionalVal<T> get_optional_not_number_val(const std::string& key, const picojson::object& src_JSON_obj) const
		{
			OptionalVal<T> ret_optional_val;

			if (src_JSON_obj.find(key) != src_JSON_obj.end()) {
				// std::mapの[]演算子は非const関数のため，at関数を用いる
				ret_optional_val.set_val(src_JSON_obj.at(key).get<T>());
				ret_optional_val.clear_omit_flag();
			}
			else {
				// キーに対応する値がなければ，デフォルト値を返す
				// デフォルト値の生成には，std::mapの[]演算子で得られるデフォルト値を返す
				std::map<std::string, T> dummy_map;
				
				ret_optional_val.set_val(dummy_map[key]);
				ret_optional_val.set_omit_flag();
			}

			return ret_optional_val;
		}

		/*
			直近に解析したJSONから，キーを指定して必須の数値を取得する関数
			引数1: const std::string& key キー名
			引数2: const picojson::object& src_JSON_obj 値の取得元となるJSONオブジェクト
			返り値: T 取得した値
			例外: 指定されたキーに対応する値が存在しないとき
		*/
		template <typename T>
		T get_obligatory_number_val(const std::string& key, const picojson::object& src_JSON_obj) const
		{
			if (src_JSON_obj.find(key) != src_JSON_obj.end()) {
				// picojsonの仕様上，数値型はdouble型でしか取り扱えないので，一旦double型で取得する
				// std::mapの[]演算子は非const関数のため，at関数を用いる
				return static_cast<T>(src_JSON_obj.at(key).get<double>());
			}
			else {
				// キーに対応する値がない
				throw CHeisClientException("必須のJSONキー\"%s\"が存在しません", key.c_str());
			}
		}

		/*
			直近に解析したJSONから，キーを指定して必須の非数値(文字列型の値，真偽値型の値, オブジェクトなど)を取得する関数
			引数1: const std::string& key キー名
			引数2: const picojson::object& src_JSON_obj 値の取得元となるJSONオブジェクト
			返り値: T 取得した値
			例外: 指定されたキーに対応する値が存在しないとき
		*/
		template <typename T>
		T get_obligatory_not_number_val(const std::string& key, const picojson::object& src_JSON_obj) const
		{
			if (src_JSON_obj.find(key) != src_JSON_obj.end()) {
				// std::mapの[]演算子は非const関数のため，at関数を用いる
				return src_JSON_obj.at(key).get<T>();
			}
			else {
				// キーに対応する値がない
				throw CHeisClientException("必須のJSONキー\"%s\"が存在しません", key.c_str());
			}
		}

	// メンバ変数
	private:
		// 解析したJSONの種類
		AnalyzedJSONKind m_analyzed_JSON_kind;
		// 解析した結果得られたJSONオブジェクト(最上位のオブジェクト)
		picojson::object m_analyzed_JSON_root_obj;
};