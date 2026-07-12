# c-language-studies

C言語で学ぶ「基本情報技術者試験アルゴリズム学習リポジトリ」
～Go/Kotlinとの比較でさらに理解を深める～

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
│   ├── go/        実装済み（記事は準備中）
│   └── kotlin/    実装済み（記事は準備中）
└── 02_search/
    ├── c/         実装済み
    ├── go/        準備中
    └── kotlin/    準備中
```

`01_bubble_sort` はGo/Kotlinのコードも追加済みです（Go/Kotlin移植編の記事は準備中）。
`02_search` は現時点でC言語版のみ実装しており、Go/Kotlinのフォルダには「準備中」のREADMEのみ置いています。

## 環境

- 言語: C（C11想定）/ Go / Kotlin
- C: gcc
- Go: 標準の `go` コマンド（Codespacesに標準で導入済み）
- Kotlin: `kotlinc`（Codespacesには標準で入っていないため、下記のインストールが必要）
- 動作確認: GitHub Codespaces / ローカル環境

```bash
# C
gcc -Wall -Wextra -o main main.c
./main

# Go
go run main.go

# Kotlin（初回のみ、SDKMAN経由でインストールが必要）
source "/usr/local/sdkman/bin/sdkman-init.sh"
sdk install kotlin

kotlinc main.kt -include-runtime -d main.jar
java -jar main.jar
```

各言語ディレクトリ（`c/` `go/` `kotlin/`）内のREADMEにも、そのディレクトリ固有の実行方法・比較ポイントを記載しているので、あわせて参照してください。

## 更新方針

コミットメッセージは、日本語で「何をしたか」が分かれば自由な書き方でOKとします。

例：
- `bubble_sortを追加`
- `READMEの表記を修正`
- `.gitignoreにjarファイルを追加`

記事に対応する大きな区切りのコミット（例：新しい言語版を追加した、など）だけ、
「どの記事に対応するか」が分かる一言を添えて、後から見返しやすくします。

例：`FE-algorithm/01_bubble_sort: Go版を追加（記事3本目対応）`
