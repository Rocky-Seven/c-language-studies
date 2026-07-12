# Kotlin版

C言語版のバブルソートをKotlinに移植したものです。

## 実行方法（Codespaces等、Kotlin導入済み環境で）

```bash
kotlinc bubble_sort.kt -include-runtime -d bubble_sort.jar
java -jar bubble_sort.jar
```

## C言語版との違い

- **交換処理**：`also` を使うと、一時変数を明示的に書かずに1行で交換できます（`arr[j] = arr[j + 1].also { arr[j + 1] = arr[j] }`）。読みやすさとのトレードオフがあるので、あえて一時変数を使うスタイルと比較してみるのも面白いポイントです
- **配列の扱い**：`IntArray` というプリミティブ型専用の配列クラスを使うと、ボクシングが発生せずCの配列に近い感覚で扱えます
- **表示処理**：`arr.joinToString(" ")` のように、配列操作用の関数が標準ライブラリに用意されているため、ループを自分で書かなくても済む場面が多くあります

