# FE-algorithm

基本情報技術者試験・午後アルゴリズム問題の実装をまとめたディレクトリです。
各アルゴリズムをまずC言語で実装・記事化し、本数がまとまった段階でGo/Kotlinへの移植記事につなげる方針で進めています。

## ディレクトリ構成

| ディレクトリ | 内容 | 実装状況 |
| --- | --- | --- |
| [`01_bubble_sort/`](./01_bubble_sort) | バブルソート | C / Go / Kotlin ともに実装済み |
| [`02_search/`](./02_search) | 線形探索・二分探索 | C / Go / Kotlin ともに実装済み |

## 各アルゴリズムの言語別実装状況

```
FE-algorithm/
├── 01_bubble_sort/
│   ├── c/         実装済み
│   ├── go/        実装済み
│   └── kotlin/    実装済み
└── 02_search/
    ├── c/         実装済み
    ├── go/        実装済み
    └── kotlin/    実装済み
```

言語ごとの実行方法や、C言語版との違い（比較ポイント）は、各アルゴリズムディレクトリ配下の `c/` `go/` `kotlin/` それぞれのREADMEに記載しています。

## 対応記事

- C言語で追う！基本情報技術者試験のトレース問題（バブルソート編）
- C言語で追う！基本情報技術者試験のトレース問題（線形探索・二分探索編）
- [バブルソートで比べる C・Go・Kotlin](https://my-studies.org/fe-comparing-bubble-sort-c-go-and-kotlin/)
- 線形探索・二分探索で比べる C・Go・Kotlin

※ 各記事のURLは、ブログ [my-studies.org](https://my-studies.org/category/sikaku/computer/programming/c-language/) の該当カテゴリーから確認できます。
