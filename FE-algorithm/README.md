# FE-algorithm

基本情報技術者試験・午後アルゴリズム問題の実装をまとめたディレクトリです。
各アルゴリズムをまずC言語で実装・記事化し、本数がまとまった段階でGo/Kotlinへの移植記事につなげる方針で進めています。

## ディレクトリ構成

| ディレクトリ | 内容 | 実装状況 |
| --- | --- | --- |
| `01_bubble_sort/` | バブルソート | C / Go / Kotlin ともに実装済み |
| `02_search/` | 線形探索・二分探索 | C / Go / Kotlin ともに実装済み |
| `03_selection_sort/` | 選択ソート | C / Go / Kotlin ともに実装済み  |
| `04_insertion_sort/` | 挿入ソート | C / Go / Kotlin ともに実装済み  |

## 各アルゴリズムの言語別実装状況

```
FE-algorithm/
├── 01_bubble_sort/
│   ├── c/         実装済み
│   ├── go/        実装済み
│   └── kotlin/    実装済み
├── 02_search/
│   ├── c/         実装済み
│   ├── go/        実装済み
│   └── kotlin/    実装済み
├── 03_selection_sort/
│   ├── c/         実装済み
│   ├── go/        実装済み
│   └── kotlin/    実装済み
└── 04_insertion_sort/
    ├── c/         実装済み
    ├── go/        実装済み
    └── kotlin/    実装済み
```

言語ごとの実行方法や、C言語版との違い（比較ポイント）は、各アルゴリズムディレクトリ配下の `c/` `go/` `kotlin/` それぞれのREADMEに記載しています。

## 対応記事

- [【基本情報技術者試験】バブルソート問題をC言語のコードで確認してみよう！](https://my-studies.org/fe-lets-examine-the-bubble-sort-problem-using-c-code/)
- [【基本情報技術者試験】C言語で追う！トレース問題（線形探索・二分探索編）](https://my-studies.org/fe-tracing-code-in-c-trace-problems-linear-search-binary-search/)
- [【基本情報技術者試験】バブルソートで比べる C・Go・Kotlin ～同じアルゴリズムでも書き方はこんなに変わる～](https://my-studies.org/fe-comparing-bubble-sort-c-go-and-kotlin/)
- [【基本情報技術者試験】線形探索・二分探索で比べる C・Go・Kotlin ～「探し方」の違いが書き方にも表れる～](https://my-studies.org/fe-comparing-linear-search-and-binary-search-in-c-go-and-kotlin/)
- [【基本情報技術者試験】選択ソート問題をC言語のコードで確認してみよう！](https://my-studies.org/fe-lets-examine-the-selection-sort-problem-using-c-code/)
- [【基本情報技術者試験】挿入ソート問題をC言語のコードで確認してみよう！](https://my-studies.org/fe-lets-examine-the-insertion-sort-problem-using-c-code/)
- [【基本情報技術者試験】選択ソートで比べる C・Go・Kotlin ～交換のロジックは共通、書き味は言語ごとに違う～](https://my-studies.org/fe-comparing-selection-sort-in-c-go-and-kotlin-shared-swap-logic-distinct-coding-styles/)
- [【基本情報技術者試験】挿入ソートで比べる C・Go・Kotlin ～シフトと挿入、3言語で見る組み方の違い～](https://my-studies.org/fe-comparing-insertion-sort-in-c-go-and-kotlin-differences-in-implementation-across-three-languagesshifting-and-inserting/)

※ 各記事のURLは、ブログ [my-studies.org](https://my-studies.org/category/sikaku/computer/programming/c-language/) の該当カテゴリーから確認できます。
