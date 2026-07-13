# Kotlin版

C言語版の線形探索・二分探索をKotlinに移植したものです。

## 実行方法（Codespaces等で）

Codespacesの標準環境にはKotlinコンパイラ（`kotlinc`）が入っていないため、初回のみインストールが必要です。

### 1. Kotlinのインストール（初回のみ）

```bash
source "/usr/local/sdkman/bin/sdkman-init.sh"
sdk install kotlin
```

すでにインストール済みかどうかは、以下で確認できます。

```bash
kotlinc -version
```

### 2. コンパイル・実行

```bash
kotlinc search.kt -include-runtime -d search.jar
java -jar search.jar
```

### 3. うまくいかない場合（Javaのバージョンエラー）

環境によっては、以下のようなエラーでコンパイルが失敗することがあります。

exception: java.lang.IllegalArgumentException: 25.0.2

これは、Kotlinコンパイラが新しすぎるJavaのバージョン表記を認識できないために起きる不具合です。SDKMANで少し古めのJava（21系）に切り替えることで解決します。

```bash
sdk install java 21.0.5-tem
sdk use java 21.0.5-tem
```

切り替え後、`java -version` で `21.0.5` になっていることを確認してから、もう一度コンパイル・実行してみてください。

> 補足：`sdk use` はそのターミナルのセッション限定で切り替わります。常に21系を使いたい場合は `sdk default java 21.0.5-tem` を使ってください。

## C言語版との違い

- **配列の走査**：Cでは添字 `i` を自分で管理して `arr[i]` を取り出しますが、Kotlinは `for (i in arr.indices)` で「有効な添字の範囲」だけを取り出せます
- **配列の扱い**：`IntArray` というプリミティブ型専用の配列クラスを使うと、ボクシングが発生せずCの配列に近い感覚で扱えます
- **分岐処理**：二分探索の「一致／小さい／大きい」の3分岐は、`if-else` の代わりに `when` 式でまとめて書けます
- **表示処理**：`"比較${comparisons}回目..."` のように、文字列の中に直接変数を埋め込めるので、Cの `printf` のようにフォーマット指定子と変数の対応を数える必要がありません
