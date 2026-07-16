# mini-diary-app

対応記事: 「C言語で ミニ日記アプリを作ろう！」連載（Ver.1〜）

これまでの連載で作成してきたミニ日記アプリのソースコードを、バージョンごとに整理してここに追加していく。

## 構成

```
mini-diary-app/
├── v1_basic_input/         Ver.1: 最低限の機能（入力・保存）
├── v2_weekday/             Ver.2: 曜日表示（日本語対応）
├── v3_read_and_recent/     Ver.3: ファイル読み込み＋過去2件表示（構造体＋関数化）
├── v4_menu/                Ver.4: メニュー方式（過去2件表示／新規入力／終了）
├── v4_1_fix/               Ver.4.1: Ver.4のタイムスタンプ二重カッコ（]]）バグを修正 ※GitHub内のみの修正版（対応するブログ記事なし）
├── v5_monthly_list/        Ver.5: 月ごとの一覧表示
├── v5_1_fix/               Ver.5.1: タイムスタンプ二重カッコ修正＋タイムゾーン（JST）固定 ※GitHub内のみの整備版（対応するブログ記事なし）
├── v6_utf8_multiline/      Ver.6: 複数行入力・UTF-8文字数カウント・100文字制限
└── v6_1_partial_notify/    Ver.6.1: 上限超過時の「部分採用＋通知」方式
```

各バージョンのディレクトリには、そのバージョンのソースコード（`.c`）と、対応記事へのリンク・機能概要をまとめた `README.md` を格納している。

## 対応記事一覧

| バージョン | 記事タイトル | URL |
| --- | --- | --- |
| Ver.1 | C言語で ミニ日記アプリを作ろう！（1） | https://my-studies.org/lets-make-a-mini-diary-app-using-c-1/ |
| Ver.2 | C言語で ミニ日記アプリを作ろう！（2） | https://my-studies.org/lets-make-a-mini-diary-app-using-c-02/ |
| Ver.3 | C言語で ミニ日記アプリを作ろう！（3） | https://my-studies.org/lets-make-a-mini-diary-app-using-c-03/ |
| Ver.4 | C言語で ミニ日記アプリを作ろう！（4） | https://my-studies.org/lets-make-a-mini-diary-app-using-c-04/ |
| Ver.5 | C言語で ミニ日記アプリを作ろう！（5） | https://my-studies.org/lets-make-a-mini-diary-app-using-c-05/ |
| Ver.6 | C言語で ミニ日記アプリを作ろう！（6） | https://my-studies.org/lets-make-a-mini-diary-app-using-c-06/ |
| Ver.6.1 | C言語で ミニ日記アプリを作ろう！（6.1） | https://my-studies.org/lets-make-a-mini-diary-app-using-c-06-01/ |

過去の記事のコードは順次移行し、以降の新しい記事はここに直接追加していく。ブログ記事の連載構成（Ver.1〜Ver.6.1）自体は変更しない。

## GitHub内のみの修正版について

`v4_1_fix`（Ver.4.1）は、Ver.4のコードに含まれていたタイムスタンプの二重カッコ（`]]`）バグを修正したものである。

`v5_1_fix`（Ver.5.1）は、Ver.5をベースに、(1) 同じ二重カッコバグの修正、(2) タイムゾーンをJST（Asia/Tokyo）に固定する修正、の2点を反映した整備版である。ブログ公開（2025.08.11）から約1年が経過していることを踏まえた所要の整備であり、これらの修正はブログ記事化はせず、GitHubリポジトリ内でのみ管理する。対応するブログ記事は存在しない。

**Ver.6以降について（`v6_2_fix`は作成していない）**：Ver.6は、`read_diary_entries()`がタイムスタンプ行を含む本文全体をそのまま1つの文字列として保持し、表示時もそのまま出力する構造に変更されており、この構造変更の副産物として、Ver.3〜Ver.5にあった「タイムスタンプ二重カッコ」バグはVer.6の時点で自然に解消されている。また、`main()`冒頭の`setenv("TZ", "Asia/Tokyo", 1); tzset();`もVer.6でブログ記事内に正式に導入済みである。したがって、`v4_1_fix`/`v5_1_fix`に相当するGitHub限定の追加修正（`v6_2_fix`）は不要と判断し、作成していない。

