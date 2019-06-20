// heis JSON解析クラス
// Author: Ryo Konno
#pragma once

#include "JSON_data_packet.h"
#include "picojson.h"

class CJSONAnalyzer{
	// 構造体，列挙体など
	public:
		// JSONの種別
		enum JSONKind {
			JSONKind_Field,				// 「盤面」JSON
			JSONKind_Result,			// 「結果」JSON
			JSONKind_NameDecided,		// 「名前確定」JSON
			JSONKind_Message,			// 「メッセージ」JSON
			JSONKind_UnknownJSON,		// 未定義のJSON
			JSONKind_NoJSONAnalyzed,	// まだJSONを解析していない
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
		JSONKind get_analyzed_JSON_kind();

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
		JSONKind distinguish_analyzed_JSON_kind() const;

		// 解析したJSONと要求するJSONの種類が一致しているか
		void validate_JSON_kind(const JSONKind req_JSON_kind) const;

		// テンプレート関数
		// TODO: 必須でない値も適切に処理できるようにする(現時点での実装だと，必須でない値が欠けた時，例外が投げられてしまう)
		/*
			直近に解析したJSONから，キーを指定して数値型の値を取得する関数
			引数1: const std::string& key キー名
			引数2: const picojson::object& src_JSON_obj 値の取得元となるJSONオブジェクト
			返り値: T 取得した値
			例外: 指定されたキーに対応する値が存在しないとき
		*/
		template <typename T>
		T get_number_val_from_JSON_obj(const std::string& key, const picojson::object& src_JSON_obj) const
		{
			if (src_JSON_obj.find(key) != src_JSON_obj.end()) {
				// 指定したキーがある場合のみ，解析したJSONから要素を取得する
				// picojsonの仕様上，数値型はdouble型でしか取り扱えないので，一旦double型で取得する
				/*
					picojsonの実装上，[]演算子を用いてobjectからvalueを取り出す場合，objectはconstにできない(constにするとコンパイルエラーになる)
					しかし，引数のsrc_JSON_objをconstなしにしてしまうと，呼び出し側で不一致が生じる可能性がある(値の変更がない関数なのに引数がconstでないなど)
					これを防ぐため，const_castで一旦const修飾子を外す
				*/
				return static_cast<T>(const_cast<picojson::object&>(src_JSON_obj)[key].get<double>());
			}
			else {
				// キーに対応する値がない
				throw std::runtime_error("JSONが不正です(指定されたキーが存在しません)");
			}
		}

		/*
			直近に解析したJSONから，キーを指定して非数値型(文字列型，真偽値型, オブジェクトなど)の値を取得する関数
			引数1: const std::string& key キー名
			引数2: const picojson::object& src_JSON_obj 値の取得元となるJSONオブジェクト
			返り値: T 取得した値
			例外: 指定されたキーに対応する値が存在しないとき
		*/
		template <typename T>
		T get_not_number_val_from_JSON_obj(const std::string& key, const picojson::object& src_JSON_obj) const
		{
			if (src_JSON_obj.find(key) != src_JSON_obj.end()) {
				// 指定したキーがある場合のみ，解析したJSONから要素を取得する
				/*
					picojsonの実装上，[]演算子を用いてobjectからvalueを取り出す場合，objectはconstにできない(constにするとコンパイルエラーになる)
					しかし，引数のsrc_JSON_objをconstなしにしてしまうと，呼び出し側で不一致が生じる可能性がある(値の変更がない関数なのに引数がconstでないなど)
					これを防ぐため，const_castで一旦const修飾子を外す
				*/
				return const_cast<picojson::object&>(src_JSON_obj)[key].get<T>();
			}
			else {
				// キーに対応する値がない
				throw std::runtime_error("JSONが不正です(指定されたキーが存在しません)");
			}
		}

	// メンバ変数
	private:
		// 解析したJSONの種類
		JSONKind m_analyzed_JSON_kind;
		// 解析した結果得られたJSONオブジェクト(最上位のオブジェクト)
		picojson::object m_analyzed_JSON_root_obj;
};