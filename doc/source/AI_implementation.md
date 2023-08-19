# AI実装

Compreheisive Client(CC)上でAIを実装する方法の説明です。

## 実装方法

CC上でAIを実装する基本的な流れを以下に示します。

1. CCの`CAIBase`クラスを継承したAIクラスを作成する。

2. AIクラスに、AIの思考ルーチンを実装する。

3. CCの`CAIFactory`クラスに、実装したAIクラスのインスタンス化処理を追加する。

1.について、特に特別な理由がない限り、ソース、ヘッダファイルともに`HeisClient_CC/src/AI`フォルダに作成することを推奨します。

2.について、思考ルーチンは`AI_main`関数に実装してください。`AI_main`関数は`CAIBase`クラスの純粋仮想関数であるため、必ず実装する必要があります。

`AI_main`関数は`board_pkt`引数をとります。この引数には、現在のターンの初めにサーバから送信される「盤面」パケットの情報が格納されています。

また、`CAIBase`にはメンバ変数としてCCの`CCommander`クラスのインスタンスである`m_commander`が定義されています。継承先のクラスでも、この`m_commander`変数を利用できます。

3.について、AIクラスのインスタンス化処理は`HeisClient_CC/src/AI/ai_factory.cpp`の`create_instance`関数内に実装します。

この時、AIクラスを一意に識別できるような名前(文字列)をつけてください。ここでつけた名前を設定ファイルのAI名に指定すると、実装したAIを対戦で利用できます。

以下に、`CTestAI`というAIクラスを実装し、`Test`という名前を付けた際に追加するべきコードを示します。

```cpp
// ...

// AIインスタンスを作成
if (ai_impl_name == "Sample") {
    ai_instance_addr = new CSampleAI(commander);
}
// ここから↓
else if (ai_impl_name == "Test") {
    ai_instance_addr = new CTestAI(commander);
}
// ここまで↑追加する
else {
    ai_instance_addr = NULL;
}

// ...
```

## 利用可能なクラス一覧

AI実装時に利用可能なクラスと、それが定義されているファイルを以下に示します。

- CCommander

- CStringUtil

各クラスにおいて利用可能なメンバ関数やメンバ変数については、各クラスの実装コード、およびコメントをご参照ください。

## 利用方法

実装したAIをCCで利用するには、各ゲームモード(観戦モードは除く)の設定ファイルで、実装したAIにつけた名前(本ページの「実装方法」の項を参照)を指定してください。

設定項目については、「設定ファイル」のページをご覧ください。
