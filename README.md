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
| [`schedule-app/`](./schedule-app) | 構造体・ファイルI/O・ソートを使ったミニ・スケジュール管理アプリ（C版）※Go/Kotlin版との比較企画 | C言語で スケジュール管理アプリ（ミニ）を作ろう！（予定） |

## 多言語比較の方針（FE-algorithm配下）

各アルゴリズムは、まずC言語で実装・記事化し、ある程度のタイミングでGo/Kotlinへの移植記事につなげる方針です。
そのため `FE-algorithm/` 配下の各アルゴリズムフォルダは、`c/` `go/` `kotlin/` という3言語構成を基本としていますが、`go/` `kotlin/` はGo/Kotlin版の実装が完了したタイミングで順次追加しています。

現時点の言語別の実装状況・対応記事は [`FE-algorithm/README.md`](./FE-algorithm/README.md) にまとめているので、そちらを参照してください。

## 環境

- 言語: C（C11想定）/ Go / Kotlin
- C: gcc
- Go: 標準の `go` コマンド（Codespacesに標準で導入済み）
- Kotlin: `kotlinc`（Codespacesには標準で入っていないため、下記のインストールが必要）
- 動作確認: GitHub Codespaces / ローカル環境

```bash
# C（例: bubble_sort.c）
gcc -Wall -Wextra -o bubble_sort bubble_sort.c
./bubble_sort

# Go（例: bubble_sort.go）
go run bubble_sort.go

# Kotlin（初回のみ、SDKMAN経由でインストールが必要）
source "/usr/local/sdkman/bin/sdkman-init.sh"
sdk install kotlin
kotlinc bubble_sort.kt -include-runtime -d bubble_sort.jar
java -jar bubble_sort.jar
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