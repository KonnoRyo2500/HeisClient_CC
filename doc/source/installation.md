# インストール手順

ここでは、Compreheisive Client(CC)をインストールする手順の説明です。

## Windows

### 必要ソフトウェア

CCのインストールには、以下のソフトウェアが必要となります。

これらがインストールされていない場合は、事前にインストールしてください。

- Visual Studio

- Git

### Gitリポジトリの取得

コマンドプロンプト上で`git`コマンドを用いて、CCのGitリポジトリを取得します。

```shell
cd (任意のディレクトリ)
git clone https://github.com/KonnoRyo2500/HeisClient_CC.git
```

### ソリューションファイルを開く

Visual Studioで、`HeisClient_CC/project_win`ディレクトリにある`HeisClient_CC.sln`を開きます。

### ビルド

Visual Studioで、CCをビルドします。

### 実行確認

Visual StudioからCCを正常に実行できれば、インストール完了です。

## Linux

### 必要ソフトウェア

CCのインストールには、以下のソフトウェアが必要となります。

これらがインストールされていない場合は、事前にインストールしてください。

- CMake

- Git

### Gitリポジトリの取得

ターミナル上で`git`コマンドを用いて、CCのGitリポジトリを取得します。

```shell
cd (任意のディレクトリ)
git clone https://github.com/KonnoRyo2500/HeisClient_CC.git
```

### makeファイルの作成

ターミナル上で`cmake`コマンドを実行し、makeファイルを作成します。

```shell
cd HeisClient_CC/project_linux
cmake .
```

### ビルド

ターミナル上で`make`コマンドを実行し、CCをビルドします。

```shell
make
```

### 実行確認

ターミナルからCCを正常に実行できれば、インストール完了です。

```shell
bin/HeisClient_CC
```
