# 疑似サーバ

Compreheisive Client(CC)の補助ツールである**疑似サーバ(Pseudo Server, PS)** の説明です。

## 概要

PSは、接続してきた2つのクライアントに対して任意の文字列を送受信できるTCPサーバソフトウェアです。PSに対してCCを接続し、JSONを送受信することでCCのオンラインモードをデバッグできます。

## インストール方法

PSは、現在Linux環境でのみ動作します。

### makeファイルの作成

ターミナル上で`cmake`コマンドを実行し、makeファイルを作成します。

```shell
cd HeisClient_CC/Tool/HeisPseudoServer
cmake .
```

### ビルド

ターミナル上で`make`コマンドを実行し、PSをビルドします。

```shell
make
```

ビルドすると、`bin`フォルダに実行ファイルが生成されます。

## コマンド

PS上での文字の送受信は、**コマンド**を用いて行います。コマンドは、後述の**シナリオファイル**に記述します。

以下では、各コマンドの詳細を示します。

### sendコマンド

クライアントに文字列を送信するコマンドです。送信する文字列は、ファイルから読み込みます。

`send`コマンドのフォーマットを以下に示します。

```text
send <input_from> <filename> <client_to>
```

#### input_fromオプション

送信する文字列をどこから読み込むかを指定します。指定可能なオプションを以下に示します。

- `file`: ファイル(ファイル名は`filename`オプションで指定)

#### filenameオプション

送信する文字列が記載されているファイル名を指定します。`input_from`オプションで`file`を指定した時のみ有効です。

ファイルは、`HeisClient_CC/Tool/HeisPseudoServer/setting/files`から読み込まれます。相対パスを指定した場合も、ここが起点となります。

#### client_toオプション

どちらのクライアントに文字列を送信するかを指定します。指定可能なオプションを以下に示します。

- `first`: 最初に接続したクライアント

- `second`: 2番目に接続したクライアント

### recvコマンド

クライアントから文字列を受信するコマンドです。受信した文字列は、オプションに応じて標準出力もしくはファイルに出力できます。

`recv`コマンドのフォーマットを以下に示します。

```text
recv <output_dst> <filename> <client_from>
```

#### output_dstオプション

受信した文字列をどこに出力するかを指定します。指定可能なオプションを以下に示します。

- `print`: 標準出力

- `file`: ファイル(ファイル名は`filename`オプションで指定)

#### filenameオプション

受信した文字列を出力するファイル名を指定します。`output_dst`オプションで`file`を指定した時のみ有効です。

ファイルは、`HeisClient_CC/Tool/HeisPseudoServer/setting/files`に保存されます。相対パスを指定した場合も、ここが起点となります。

#### client_fromオプション

どちらのクライアントから文字列を受信するかを指定します。指定可能なオプションを以下に示します。

- `first`: 最初に接続したクライアント

- `second`: 2番目に接続したクライアント

## シナリオファイル

コマンドを記述するファイルです。PSは、このシナリオファイルに記載されたコマンドを上から順に実行します。

シナリオファイルは、`HeisClient_CC/Tool/HeisPseudoServer/scenario`に配置してください。

読み込むシナリオファイルは、PSの設定ファイルで指定できます。

## 設定ファイル

PSの設定項目を記述するファイルです。

パスは`HeisClient_CC/Tool/HeisPseudoServer/setting/ps_setting.csv`です。

設定項目は下表のとおりです。

| 項目名                    | 設定値の型  | 設定値の範囲  | 設定値の意味                      |
| ---------------------- | ------ | ------- | --------------------------- |
| scenario_file_name     | 文字列    | -       | シナリオファイル名(※1)。              |
| listen_port_num_first  | 数値(整数) | 0～65535 | 最初に接続するクライアントを待ち受けるポート番号。   |
| listen_port_num_second | 数値(整数) | 0～65535 | 2番目に接続するクライアントを待ち受けるポート番号。  |
| listen_addr_first      | 文字列    | -       | 最初に接続するクライアントを待ち受けるIPアドレス。  |
| listen_addr_second     | 文字列    | -       | 2番目に接続するクライアントを待ち受けるIPアドレス。 |

※1: シナリオファイルは、`HeisClient_CC/Tool/HeisPseudoServer/scenario`から読み込まれます。相対パスを指定した場合も、ここが起点となります。

## 利用方法

まず、コマンドプロンプトでPSを起動します。

```shell
bin/HeisPseudoServer
```

PSが起動したら、PSは接続待ち状態になります。

そうしたら、CCを2つオンラインモードで起動して接続します。

以下の手順でCCを起動することにより、PSに2つのCCを接続することができます。

1. CCの設定ファイルを編集し、接続先IPアドレスをPS設定ファイルの`listen_addr_first`、ポート番号を`listen_port_num_first`で指定したものにする。

2. 1つ目のCCをオンラインモードで起動する。

3. CCの設定ファイルを編集し、接続先IPアドレスをPS設定ファイルの`listen_addr_second`、ポート番号を`listen_port_num_second`で指定したものにする。

4. 2つ目のCCをオンラインモードで起動する。

PSにCCを2つ接続すると、PSとCCの通信が始まります。
