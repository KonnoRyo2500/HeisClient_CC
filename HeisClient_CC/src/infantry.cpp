// heis 兵士クラス
// Author: Ryo Konno
#include "infantry.h"
#include "field.h"
#include <stdexcept>
#include <queue>

/* public関数 */

/*
	コンストラクタ
	引数1: const std::string& team_name 所属チーム名
	引数2: const std::string& infantry_ID ID
	引数3: const uint16_t init_pos_x 初期位置のx座標
	引数4: const uint16_t init_pos_y 初期位置のy座標
*/
CInfantry::CInfantry(const std::string& team_name, const std::string& infantry_ID, const uint16_t init_pos_x, const uint16_t init_pos_y)
	: m_id(infantry_ID)
	, m_hp(InitialValue_HP)
	, m_team_name(team_name)
	, m_pos_x(init_pos_x)
	, m_pos_y(init_pos_y)
	, m_attack_x(UINT16_MAX)
	, m_attack_y(UINT16_MAX)
	, m_action_remain(InitialValue_ActionRemain)
{
	// 呼ばれるタイミングによってはフィールドが未作成の場合もあるので，フィールドが作成済みであることを確認してから配置するようにする
	CField* field = CField::get_instance();
	if (field != NULL && (field->get_width() != 0 && field->get_height() != 0)) {
		field->set_infantry(init_pos_x, init_pos_y, this);
	}
}

/*
	デストラクタ
	引数なし
*/
CInfantry::~CInfantry()
{
	// 呼ばれるタイミングによってはフィールドが未作成の場合もあるので，フィールドが作成済みであることを確認してから削除するようにする
	CField* field = CField::get_instance();
	if (field != NULL && (field->get_width() != 0 && field->get_height() != 0)) {
		field->remove_infantry(m_pos_x, m_pos_y);
	}
}

/*
	兵士のチーム名を取得する関数
	引数なし
	返り値: std::string 兵士のチーム名
*/
std::string CInfantry::get_team_name() const
{
	return m_team_name;
}

/*
	兵士のIDを取得する関数
	引数なし
	返り値: std::string 兵士のID
*/
std::string CInfantry::get_id() const
{
	return m_id;
}

/*
	兵士のx座標を取得する関数
	引数なし
	返り値: uint16_t 兵士のx座標
*/
uint16_t CInfantry::get_x_position() const
{
	return m_pos_x;
}

/*
	兵士のy座標を取得する関数
	引数なし
	返り値: uint16_t 兵士のy座標
*/
uint16_t CInfantry::get_y_position() const
{
	return m_pos_y;
}

/*
	兵士の残り行動回数を取得する関数
	引数なし
	返り値: uint8_t 兵士の残り行動回数
*/
uint8_t CInfantry::get_action_remain() const
{
	return m_action_remain;
}

/*
	兵士のHPを取得する関数
	引数なし
	返り値: int8_t 兵士のHP
*/
int8_t CInfantry::get_hp() const
{
	return m_hp;
}


/*
	指定した方向に攻撃を行う関数
	引数1: const Direction direction 攻撃方向
	返り値なし
*/
void CInfantry::attack(const Direction direction)
{
	if (m_action_remain <= 0) {
		fprintf(stderr, "この兵士は行動できません\n");
		return;
	}

	CField* field = CField::get_instance();

	try {
		CInfantry* dst_infantry = field->get_infantry(get_neighbor_x_pos(direction), get_neighbor_y_pos(direction));
		if (dst_infantry != NULL) {
			if (dst_infantry->get_team_name() == m_team_name) {
				// 範囲外の位置に攻撃しようとした場合，自身の座標に攻撃することになるため，ここに処理が移る
				fprintf(stderr, "味方の兵士を攻撃しようとしています\n");
				return;
			}

			// 相手兵士のHPを減少させる
			dst_infantry->attacked();

			// 攻撃先を更新
			m_attack_x = get_neighbor_x_pos(direction);
			m_attack_y = get_neighbor_y_pos(direction);

			// 攻撃したら，それ以降兵士は行動不可能
			m_action_remain = 0;
		}
		else {
			fprintf(stderr, "攻撃しようとした方向に兵士がいません\n");
		}
	}
	catch (std::exception & e) {
		fprintf(stderr, "攻撃に失敗しました\n理由: %s\n", e.what());
	}
}

/*
	兵士を移動する関数
	引数1: const int16_t delta_x x方向に何マス移動するか(正の値であれば下方向, 負の値であれば上方向)
	引数2: const int16_t delta_y y方向に何マス移動するか(正の値であれば右方向, 負の値であれば左方向)
	返り値なし
*/
void CInfantry::move(const int16_t delta_x, const int16_t delta_y)
{
	if (m_action_remain <= 0) {
		fprintf(stderr, "この兵士は行動できません\n");
		return;
	}

	CField* field = CField::get_instance();
	uint16_t dst_x = m_pos_x + delta_x, dst_y = m_pos_y + delta_y;

	if (calc_L1_distance(m_pos_x, m_pos_y, dst_x, dst_y) > m_action_remain) {
		fprintf(stderr, "移動できる歩数の上限を超えて移動しようとしています\n");
		return;
	}
	try {
		CInfantry* dst_infantry = field->get_infantry(dst_x, dst_y);
		if (dst_infantry == NULL) {
			if (exists_path_for_move(dst_x, dst_y)) {
				// フィールド上の座標を更新する
				field->remove_infantry(m_pos_x, m_pos_y);
				field->set_infantry(dst_x, dst_y, this);

				// 移動した歩数分だけ，行動回数を減らす
				m_action_remain -= calc_L1_distance(m_pos_x, m_pos_y, dst_x, dst_y);

				// 自身の位置を更新する
				m_pos_x = dst_x;
				m_pos_y = dst_y;
			}
			else {
				fprintf(stderr, "移動経路がありません\n");
			}
		}
		else {
			fprintf(stderr, "移動先に兵士がいます\n");
		}
	}
	catch (std::exception & e) {
		fprintf(stderr, "移動に失敗しました\n理由: %s\n", e.what());
	}
}

/*
	自分の周囲4マスを探索する関数
	引数なし
	返り値: std::vector<CInfantry::NeighborInfantryData> 周囲のマスにいる兵士の情報(最大4マス)
*/
std::vector<CInfantry::NeighborInfantryData> CInfantry::look_around() const
{
	std::vector<NeighborInfantryData> neighbor_infantries;
	CField* field = CField::get_instance();

	for (auto direction : {Direction_Up, Direction_Down, Direction_Left, Direction_Right}) {
		CInfantry* infantry = field->get_infantry(get_neighbor_x_pos(direction), get_neighbor_y_pos(direction));

		// ある方向で，自分のいるマスを参照していたら，その方向にはマスが存在していない
		if (!is_self(infantry)) {
			NeighborInfantryData infantry_data = {direction, infantry};

			neighbor_infantries.push_back(infantry_data);
		}
	}

	return neighbor_infantries;
}

/*
	「行動」パケットの"contents"配列の1要素分のデータを作成する関数
	引数なし
	返り値: ContentsArrayElem "contents"配列の要素
*/
ContentsArrayElem CInfantry::create_contents_array_elem() const
{
	ContentsArrayElem contents_elem;

	contents_elem.unit_id = m_id;
	contents_elem.to_x = m_pos_x;
	contents_elem.to_y = m_pos_y;
	if (m_attack_x != UINT16_MAX && m_attack_y != UINT16_MAX) {
		// ターン内に攻撃していれば，攻撃先を設定する
		contents_elem.atk_x = m_attack_x;
		contents_elem.atk_y = m_attack_y;
	}
	else {
		// 攻撃していなければ，現在位置と同じ座標を設定する
		contents_elem.atk_x = m_pos_x;
		contents_elem.atk_y = m_pos_y;
	}
	return contents_elem;
}

/* private関数 */

/*
	敵の兵士からの攻撃を反映させる関数
	引数なし
	返り値なし
*/
void CInfantry::attacked()
{
	m_hp--;
	// 自身の体力が尽きたら，死ぬ(自身をフィールドから消去する)
	if (m_hp <= 0) {
		CField::get_instance()->remove_infantry(m_pos_x, m_pos_y);
	}
}

/*
	指定されたマスに移動するための経路があるかどうかを判定する関数
	引数1: const uint16_t dst_x 目的マスのx座標
	引数2: const uint16_t dst_y 目的マスのy座標
	返り値: 目的マスに移動できるか
	備考: 目的マスに兵士はいないものとする(この関数を呼び出す際，目的マスに兵士がいるかどうかを考慮済みとする)
*/
bool CInfantry::exists_path_for_move(const uint16_t dst_x, const uint16_t dst_y) const
{
	// 移動可能なマスの一覧
	std::queue<uint16_t> available_pos_x, available_pos_y;
	uint16_t current_x, current_y;

	available_pos_x.push(m_pos_x);
	available_pos_y.push(m_pos_y);
	while(available_pos_x.size() > 0 && available_pos_y.size() > 0){
		/*
			探索の手順
			(1)現在位置から上下左右に隣接しているマスで，目的マスに近づけるマスをすべて見る
			(2)その中で，移動可能なマスがあれば，それを記録する
			(3)記録したマスの中で，目的マスと同じマスがあれば，探索成功
			(4)そうでなければ，移動可能なマスのうち，まだ移動していないマスに現在位置を移動する(仮想的な移動とする．すなわちメンバ変数に入っている現在位置は変更しない)
			(5)このとき，移動できるマスがなければ，探索失敗
			TODO: 移動可能な歩数が4歩以上になると，敵の兵士を迂回して目的マスに移動するルートが発生するので，この手法は使えなくなる．そのときは，探索手法の再検討が必要
		*/
		// 現在位置の更新
		current_x = available_pos_x.front();
		current_y = available_pos_y.front();
		available_pos_x.pop();
		available_pos_y.pop();
		// 移動可能な隣接マスの探索
		for (Direction dir_to_dst : decide_move_direction(current_x, current_y, dst_x, dst_y)) {
			CField* field = CField::get_instance();
			CInfantry* neighbor_infantry = field->get_infantry(get_neighbor_x_pos(dir_to_dst, current_x), get_neighbor_y_pos(dir_to_dst, current_y));
			// 味方の兵士は飛び越して移動できるので，兵士がいたとしても味方ならばそのマスは移動可能とする
			if (neighbor_infantry == NULL || neighbor_infantry->get_team_name() == m_team_name) {
				if (get_neighbor_x_pos(dir_to_dst, current_x) == dst_x && get_neighbor_y_pos(dir_to_dst, current_y) == dst_y) {
					return true;
				}
				else {
					available_pos_x.push(get_neighbor_x_pos(dir_to_dst, current_x));
					available_pos_y.push(get_neighbor_y_pos(dir_to_dst, current_y));
				}
			}
		}
	}
	return false;
}

/*
	目的マスに近づける移動方向を判定する関数
	引数1: const uint16_t current_x 現在いるマスのx座標
	引数2: const uint16_t current_y 現在いるマスのy座標
	引数3: const uint16_t dst_x 目的マスのx座標
	引数4: const uint16_t dst_y 目的マスのy座標
	返り値: std::vector<Direction> 現在いるマスから目的マスに近づける移動方向
*/
std::vector<Direction> CInfantry::decide_move_direction(const uint16_t current_x, const uint16_t current_y, const uint16_t dst_x, const uint16_t dst_y) const
{
	std::vector<Direction> directions_to_dst;

	if (current_x < dst_x) {
		directions_to_dst.push_back(Direction_Right);
	}
	if (current_x > dst_x) {
		directions_to_dst.push_back(Direction_Left);
	}
	if (current_y < dst_y) {
		directions_to_dst.push_back(Direction_Down);
	}
	if (current_y > dst_y) {
		directions_to_dst.push_back(Direction_Up);
	}
	// 既に目的マスにいれば，directions_to_dstに方向は入らない
	return directions_to_dst;
}

/*
	与えられた2点(x1, y1), (x2, y2)のL1距離を計算する関数
	引数1, 2: const uint16_t x1, const uint16_t y1 点(x1, y1)の座標
	引数3, 4: const uint16_t x2, const uint16_t y2 点(x2, y2)の座標
	返り値: uint16_t L1距離
*/
uint16_t CInfantry::calc_L1_distance(const uint16_t x1, const uint16_t y1, const uint16_t x2, const uint16_t y2) const
{
	return std::abs(x1 - x2) + std::abs(y1 - y2);
}

/*
	与えられた兵士が自分自身なのかを判定する関数
	引数1: const CInfantry* infantry 兵士
	返り値: bool 与えられた兵士が自分自身か
*/
bool CInfantry::is_self(const CInfantry* infantry) const
{
	if (infantry == NULL) {
		return false;
	}
	else if(infantry->get_id() == m_id){
		return true;
	}

	return false;
}

/*
	フィールドのマスのうち，指定した方向に隣接したマスのx座標を取得する関数
	引数1: const Direction direction 方向
	返り値: uint16_t x座標
	備考: 現在いるマスのx座標をを基準とする
*/
uint16_t CInfantry::get_neighbor_x_pos(const Direction direction) const
{
	uint16_t dst_x_pos;

	switch (direction) {
		case Direction_Left:
			dst_x_pos = m_pos_x - 1;
			break;
		case Direction_Right:
			dst_x_pos = m_pos_x + 1;
			break;
		default:
			dst_x_pos = m_pos_x;
			break;
	}

	if (dst_x_pos < 0) {
		dst_x_pos = 0;
	}
	// TODO: フィールドの幅を定数ではなく，フィールドから得るようにする
	else if (FieldParam_Width <= dst_x_pos) {
		dst_x_pos = FieldParam_Width - 1;
	}

	return dst_x_pos;
}

/*
	フィールドのマスのうち，指定した方向に隣接したマスのx座標を取得する関数
	引数1: const Direction direction 方向
	引数2: const uint16_t ref_x 基準となるマスのx座標
	返り値: uint16_t x座標
	備考: 基準となるマスのx座標を引数で指定できる
*/
uint16_t CInfantry::get_neighbor_x_pos(const Direction direction, const uint16_t ref_x) const
{
	uint16_t dst_x_pos;

	switch (direction) {
	case Direction_Left:
		dst_x_pos = ref_x - 1;
		break;
	case Direction_Right:
		dst_x_pos = ref_x + 1;
		break;
	default:
		dst_x_pos = ref_x;
		break;
	}

	if (dst_x_pos < 0) {
		dst_x_pos = 0;
	}
	// TODO: フィールドの幅を定数ではなく，フィールドから得るようにする
	else if (FieldParam_Width <= dst_x_pos) {
		dst_x_pos = FieldParam_Width - 1;
	}

	return dst_x_pos;
}

/*
	フィールドのマスのうち，指定した方向に隣接したマスのy座標を取得する関数
	引数1: const Direction direction 方向
	返り値: uint16_t y座標
	備考: 現在いるマスのy座標をを基準とする
*/
uint16_t CInfantry::get_neighbor_y_pos(const Direction direction) const
{
	uint16_t dst_y_pos;

	switch (direction) {
	case Direction_Up:
		dst_y_pos = m_pos_y - 1;
		break;
	case Direction_Down:
		dst_y_pos = m_pos_y + 1;
		break;
	default:
		dst_y_pos = m_pos_y;
		break;
	}

	if (dst_y_pos < 0) {
		dst_y_pos = 0;
	}
	// TODO: フィールドの高さを定数ではなく，フィールドから得るようにする
	else if (FieldParam_Height <= dst_y_pos) {
		dst_y_pos = FieldParam_Height - 1;
	}

	return dst_y_pos;
}

/*
	フィールドのマスのうち，指定した方向に隣接したマスのy座標を取得する関数
	引数1: const Direction direction 方向
	引数2: const uint16_t ref_y 基準となるマスのy座標
	返り値: uint16_t y座標
	備考: 基準となるマスのy座標を引数で指定できる
*/
uint16_t CInfantry::get_neighbor_y_pos(const Direction direction, const uint16_t ref_y) const
{
	uint16_t dst_y_pos;

	switch (direction) {
	case Direction_Up:
		dst_y_pos = ref_y - 1;
		break;
	case Direction_Down:
		dst_y_pos = ref_y + 1;
		break;
	default:
		dst_y_pos = ref_y;
		break;
	}

	if (dst_y_pos < 0) {
		dst_y_pos = 0;
	}
	// TODO: フィールドの高さを定数ではなく，フィールドから得るようにする
	else if (FieldParam_Height <= dst_y_pos) {
		dst_y_pos = FieldParam_Height - 1;
	}

	return dst_y_pos;
}