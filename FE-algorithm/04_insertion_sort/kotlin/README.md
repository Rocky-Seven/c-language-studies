# Kotlin版
C言語版の挿入ソートをKotlinに移植したものです。
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
kotlinc InsertionSort.kt -include-runtime -d insertion_sort.jar
java -jar insertion_sort.jar
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
## 実行結果例
```
ソート前: 5 2 8 1 9 3
1回目のパス後: 2 5 8 1 9 3
2回目のパス後: 2 5 8 1 9 3
3回目のパス後: 1 2 5 8 9 3
4回目のパス後: 1 2 5 8 9 3
5回目のパス後: 1 2 3 5 8 9
ソート後: 1 2 3 5 8 9
```
## C言語版との違い
- **ループの表現**：`while` 文はCと同じ書き方がそのまま使えます（Goのようにforで代用する必要がありません）
- **配列の扱い**：`IntArray` というプリミティブ型専用の配列クラスを使うと、ボクシングが発生せずCの配列に近い感覚で扱えます
- **表示処理**：`"${data.joinToString(" ")}"` のように、文字列の中に直接変数を埋め込めるので、Cの `printf` のようにフォーマット指定子と変数の対応を数える必要がありません
