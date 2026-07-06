# c-language-study

ブログ [my-studies.org](https://my-studies.org/category/sikaku/computer/programming/c-language/) の「C言語」カテゴリーで扱っているコードをまとめたリポジトリです。
記事ごとにディレクトリを分け、各ディレクトリのソースコードと解説がブログ記事の内容と対応するようにしています。

## ディレクトリ構成

| ディレクトリ | 内容 | 対応する記事シリーズ |
| --- | --- | --- |
| [`mini-diary-app/`](./mini-diary-app) | 構造体・ファイルI/Oを使ったミニ日記アプリ | C言語で ミニ日記アプリを作ろう！（連載） |
| [`FE-algorithm/`](./FE-algorithm) | 基本情報技術者試験・午後アルゴリズム問題の実装 | 基本情報技術者試験アルゴリズム問題（連載） |
| [`holiday-calendar-app/`](./holiday-calendar-app) | 祝日対応カレンダーアプリ（C版）※Go/Kotlin/Rust版との比較企画 | C言語で 祝日カレンダーアプリを作ろう！（予定） |

## 多言語比較の方針（FE-algorithm配下）

各アルゴリズムは、まずC言語で実装・記事化し、本数がまとまった段階でGo/Kotlinへの移植記事につなげる方針です。
そのため `FE-algorithm/` 配下の各アルゴリズムフォルダは、あらかじめ以下の構成を取っています。

```
FE-algorithm/
├── 01_bubble_sort/
│   ├── c/         実装済み
│   ├── go/        準備中
│   └── kotlin/    準備中
└── 02_search/
    ├── c/         執筆予定
    ├── go/        準備中
    └── kotlin/    準備中
```

現時点ではC言語版のみ実装し、Go/Kotlinのフォルダには「準備中」のREADMEのみ置いています。

## 環境

- 言語: C（C11想定）
- コンパイラ: gcc
- 動作確認: GitHub Codespaces / ローカル環境

```bash
gcc -Wall -Wextra -o main main.c
./main
```

## 更新方針

各記事の公開に合わせて、対応するディレクトリにコードを追加・更新していきます。
コミットメッセージは `記事タイトル or 機能名: 変更内容` の形式を基本とします。

例）`bubble_sort: 初回実装（記事1本目）`
