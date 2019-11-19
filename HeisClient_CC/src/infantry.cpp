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
	引数2: const std::string& infantry_id ID
	引数3: const FieldPosition& init_pos 初期位置
*/
CInfantry::CInfantry(const std::string& team_name, const std::string& infantry_id, const FieldPosition& init_pos)
	: m_id(infantry_id)
	, m_hp(InitialValue_HP)
	, m_team_name(team_name)
	, m_pos(init_pos)
	, m_attack_pos({UINT16_MAX, UINT16_MAX})
	, m_action_remain(InitialValue_ActionRemain)
{
	// 呼ばれるタイミングによってはフィールドが未作成の場合もあるので，フィールドが作成済みであることを確認してから配置するようにする
	CField* field = CField::get_instance();
	if (field != NULL && (field->get_width() != 0 && field->get_height() != 0)) {
		field->set_infantry(init_pos, this);
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
		field->remove_infantry(m_pos);
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
	兵士の座標を取得する関数
	引数なし
	返り値: FieldPosition 兵士の座標
*/
FieldPosition CInfantry::get_position() const
{
	return m_pos;
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
	引数1: const FieldPosition dst_pos 攻撃先の座標
	返り値なし
*/
void CInfantry::attack(const FieldPosition dst_pos)
{
	if (m_action_remain <= 0) {
		fprintf(stderr, "この兵士は行動できません\n");
		return;
	}
	if (calc_L1_distance(m_pos, dst_pos) != 1) {
		fprintf(stderr, "攻撃先座標の指定が不正です\n");
		return;
	}

	CField* field = CField::get_instance();

	try {
		CInfantry* dst_infantry = field->get_infantry(dst_pos);
		if (dst_infantry != NULL) {
			if (dst_infantry->get_team_name() == m_team_name) {
				fprintf(stderr, "味方の兵士を攻撃しようとしています\n");
				return;
			}

			// 相手兵士のHPを減少させる
			dst_infantry->attacked();

			// 攻撃先を更新
			m_attack_pos = dst_pos;

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
	引数1: const FieldPosition dst_pos 移動先の座標
	返り値なし
*/
void CInfantry::move(const FieldPosition dst_pos)
{
	if (m_action_remain <= 0) {
		fprintf(stderr, "この兵士は行動できません\n");
		return;
	}

	CField* field = CField::get_instance();

	if (calc_L1_distance(m_pos, dst_pos) > m_action_remain) {
		fprintf(stderr, "移動できる歩数の上限を超えて移動しようとしています\n");
		return;
	}
	try {
		CInfantry* dst_infantry = field->get_infantry(dst_pos);
		if (exists_path_for_move(dst_pos)) {
			// フィールド上の座標を更新する
			field->remove_infantry(m_pos);
			field->set_infantry(dst_pos, this);

			// 移動した歩数分だけ，行動回数を減らす
			m_action_remain -= calc_L1_distance(m_pos, dst_pos);

			// 自身の位置を更新する
			m_pos = dst_pos;
		}
		else {
			fprintf(stderr, "移動経路がありません\n");
		}
	}
	catch (std::exception & e) {
		fprintf(stderr, "移動に失敗しました\n理由: %s\n", e.what());
	}
}

/*
	移動可能なすべてのマスを返す関数
	引数なし
	返り値: std::vector<FieldPosition> 移動可能なマス
*/
std::vector<FieldPosition> CInfantry::find_movable_position() const
{
	std::vector<FieldPosition> movable_pos;

	// 移動可能なマスをすべて探索する
	for (FieldPosition pos : get_around_position(m_action_remain)) {
		if (exists_path_for_move(pos)) {
			movable_pos.push_back(pos);
		}
	}
	
	return movable_pos;
}

/*
	攻撃可能なすべてのマスを返す関数
	引数なし
	返り値: std::vector<FieldPosition> 攻撃可能なマス
*/
std::vector<FieldPosition> CInfantry::find_attackable_position() const
{
	std::vector<FieldPosition> attackable_pos;
	CField* field = CField::get_instance();

	// 攻撃は自分に隣接したマスにしかできないので，自分の上下左右のマス(自身からのL1距離が1のマス)のみを探索すればよい
	for (FieldPosition pos : get_around_position(1)) {
		if (field->get_infantry(pos) != NULL && field->get_infantry(pos)->get_team_name() != m_team_name) {
			attackable_pos.push_back(pos);
		}
	}

	return attackable_pos;
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
	contents_elem.to_x = m_pos.x;
	contents_elem.to_y = m_pos.y;
	if (m_attack_pos.x != UINT16_MAX && m_attack_pos.y != UINT16_MAX) {
		// ターン内に攻撃していれば，攻撃先を設定する
		contents_elem.atk_x = m_attack_pos.x;
		contents_elem.atk_y = m_attack_pos.y;
	}
	else {
		// 攻撃していなければ，現在位置と同じ座標を設定する
		contents_elem.atk_x = m_pos.x;
		contents_elem.atk_y = m_pos.y;
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
		CField::get_instance()->remove_infantry(m_pos);
	}
}

/*
	指定されたマスに移動するための経路があるかどうかを判定する関数
	引数1: const FieldPosition& dst_pos 行先のマス
	返り値: 目的マスに移動できるか
*/
bool CInfantry::exists_path_for_move(const FieldPosition& dst_pos) const
{
	// 目的マスに兵士がいれば，その時点で移動経路がないことがわかる
	if (CField::get_instance()->get_infantry(dst_pos) != NULL) {
		return false;
	}

	// 移動可能なマスの一覧
	std::queue<FieldPosition> available_pos;
	FieldPosition current_pos;

	available_pos.push(m_pos);
	while(available_pos.size() > 0){
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
		current_pos = available_pos.front();
		available_pos.pop();
		// 移動可能な隣接マスの探索
		for (Direction dir_to_dst : decide_move_direction(current_pos, dst_pos)) {
			CField* field = CField::get_instance();
			CInfantry* neighbor_infantry = field->get_infantry(get_neighbor_pos(dir_to_dst, current_pos));
			// 味方の兵士は飛び越して移動できるので，兵士がいたとしても味方ならばそのマスは移動可能とする
			if (neighbor_infantry == NULL || neighbor_infantry->get_team_name() == m_team_name) {
				if (get_neighbor_pos(dir_to_dst, current_pos) == dst_pos) {
					return true;
				}
				else {
					available_pos.push(get_neighbor_pos(dir_to_dst, current_pos));
				}
			}
		}
	}
	return false;
}

/*
	目的マスに近づける移動方向を判定する関数
	引数1: const FieldPosition& current_pos 現在いるマスの座標
	引数2: const FieldPosition& dst_pos 目的マスの座標
	返り値: std::vector<Direction> 現在いるマスから目的マスに近づける移動方向
*/
std::vector<Direction> CInfantry::decide_move_direction(const FieldPosition& current_pos, const FieldPosition& dst_pos) const
{
	std::vector<Direction> directions_to_dst;

	if (current_pos.x < dst_pos.x) {
		directions_to_dst.push_back(Direction_Right);
	}
	if (current_pos.x > dst_pos.x) {
		directions_to_dst.push_back(Direction_Left);
	}
	if (current_pos.y < dst_pos.y) {
		directions_to_dst.push_back(Direction_Down);
	}
	if (current_pos.y > dst_pos.y) {
		directions_to_dst.push_back(Direction_Up);
	}
	// 既に目的マスにいれば，directions_to_dstに方向は入らない
	return directions_to_dst;
}

/*
	与えられた2点のL1距離を計算する関数
	引数1: const FieldPosition& src 点1の座標
	引数2: const FieldPosition dst 点2の座標
	返り値: uint16_t L1距離
*/
uint16_t CInfantry::calc_L1_distance(const FieldPosition& src, const FieldPosition dst) const
{
	return std::abs(src.x - dst.x) + std::abs(src.y - dst.y);
}

/*
	自分の周囲のマスのうち，指定したL1距離以内にあるマスを取得する関数
	引数1: const uint16_t search_distance 探索範囲のL1距離の上限
	返り値: std::vector<FieldPosition> 取得したすべてのマス
*/
std::vector<FieldPosition> CInfantry::get_around_position(const uint16_t search_distance) const
{
	std::vector<FieldPosition> around_positions;
	CField* field = CField::get_instance();
	// for文の初期化式および条件式が長くなるのを防ぐため，x, yの境界値は変数に入れる
	// フィールドの範囲チェック付きで，現在地からのL1距離がsearch_distance以内のすべてのマスを探索できるようになっている
	uint16_t x_begin = (search_distance <= m_pos.x ? m_pos.x - search_distance : 0);
	uint16_t x_end = (m_pos.x + search_distance < field->get_width() ? m_pos.x + search_distance : field->get_width() - 1);

	for (uint16_t x = x_begin; x <= x_end; x++) {
		uint16_t y_begin = ((search_distance - std::abs(m_pos.x - x)) <= m_pos.y ? m_pos.y - (search_distance - std::abs(m_pos.x - x)) : 0);
		uint16_t y_end = (m_pos.y + (search_distance - std::abs(m_pos.x - x)) < field->get_height() ? m_pos.y + (search_distance - std::abs(m_pos.x - x)) : field->get_height() - 1);
		for (uint16_t y = y_begin; y <= y_end; y++) {
			// 自身のいるマスは簡単にわかるので，自分のいるマスについては探索対象としない
			if (FieldPosition(x, y) != m_pos) {
				around_positions.push_back(FieldPosition(x, y));
			}
		}
	}

	return around_positions;
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
	フィールドのマスのうち，指定した方向に隣接したマスの座標を取得する関数
	引数1: const Direction direction 方向
	引数2: const FieldPosition& origin 基準となるマスの座標
	返り値: FieldPosition 隣接したマス
	備考: 指定した方向に隣接したマスの座標がフィールドの範囲外だった場合，基準となるマスの座標を返す
*/
FieldPosition CInfantry::get_neighbor_pos(const Direction direction, const FieldPosition& origin) const
{
	FieldPosition dst_pos;

	// x座標を決定
	switch (direction) {
	case Direction_Left:
		dst_pos.x = origin.x - 1;
		break;
	case Direction_Right:
		dst_pos.x = origin.x + 1;
		break;
	default:
		dst_pos.x = origin.x;
		break;
	}
	if (dst_pos.x < 0) {
		dst_pos.x = 0;
	}
	else if (CField::get_instance()->get_width() <= dst_pos.x) {
		dst_pos.x = CField::get_instance()->get_width() - 1;
	}

	// y座標を決定
	switch (direction) {
	case Direction_Up:
		dst_pos.y = origin.y - 1;
		break;
	case Direction_Down:
		dst_pos.y = origin.y + 1;
		break;
	default:
		dst_pos.y = origin.y;
		break;
	}
	if (dst_pos.y < 0) {
		dst_pos.y = 0;
	}
	else if (CField::get_instance()->get_height() <= dst_pos.y) {
		dst_pos.y = CField::get_instance()->get_height() - 1;
	}

	return dst_pos;
}