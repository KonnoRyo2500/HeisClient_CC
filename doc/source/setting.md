# 設定ファイル

CCで利用される各種設定ファイルについての説明です。

## ローカルモード設定ファイル

ローカルモードにおける設定項目を記述するファイルです。

パスは`HeisClient_CC/setting/local_setting.csv`です。

設定項目は下表のとおりです。

| 項目名                               | 設定値の型  | 設定値の範囲         | 設定値の意味           |
| --------------------------------- | ------ | -------------- | ---------------- |
| board_width                       | 数値(整数) | 1～             | 盤面の幅。            |
| board_height                      | 数値(整数) | 1～             | 盤面の高さ。           |
| my_team_name                      | 文字列    | -              | 自チーム名。           |
| enemy_team_name                   | 文字列    | -              | 敵チーム名。           |
| my_ai_implementation              | 文字列    | -              | 自チームで利用するAIの名前。  |
| enemy_ai_implementation           | 文字列    | -              | 敵チームで利用するAIの名前。  |
| my_infantries_init_x_coodinate    | 数値(整数) | 0～board_width  | 自チームの兵士のx座標(※1)。 |
| my_infantries_init_y_coodinate    | 数値(整数) | 0～board_height | 自チームの兵士のy座標(※1)。 |
| enemy_infantries_init_x_coodinate | 数値(整数) | 0～board_width  | 敵チームの兵士のx座標(※1)。 |
| enemy_infantries_init_y_coodinate | 数値(整数) | 0～board_height | 敵チームの兵士のy座標(※1)。 |
| first_turn_team                   | 文字列    | (※2)           | 先手で行動するチーム名。     |

※1: 設定値を複数指定できます。ここで指定した設定値の個数が、盤面に配置される兵士の数になります。設定値を複数指定した場合は、左から順に兵士1の座標, 兵士2の座標, ...となります。

※2: `my_team_name`もしくは`enemy_team_name`で指定した名前を記述してください。

## オンラインモード設定ファイル

オンラインモードにおける設定項目を記述するファイルです。

パスは`HeisClient_CC/setting/online_setting.csv`です。

設定項目は下表のとおりです。

| 項目名               | 設定値の型  | 設定値の範囲  | 設定値の意味      |
| ----------------- | ------ | ------- | ----------- |
| server_ip_addr    | 文字列    | -       | サーバのIPアドレス。 |
| server_port_num   | 数値(整数) | 0～65535 | サーバのポート番号。  |
| team_name         | 文字列    | -       | チーム名。       |
| ai_implementation | 文字列    | -       | 利用するAIの名前。  |

## 観戦モード設定ファイル

観戦モードにおける設定項目を記述するファイルです。

パスは`HeisClient_CC/setting/audience_setting.csv`です。

設定項目は下表のとおりです。

| 項目名             | 設定値の型  | 設定値の範囲  | 設定値の意味      |
| --------------- | ------ | ------- | ----------- |
| server_ip_addr  | 文字列    | -       | サーバのIPアドレス。 |
| server_port_num | 数値(整数) | 0～65535 | サーバのポート番号。  |
