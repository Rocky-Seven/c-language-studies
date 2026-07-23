# schedule-mini-app

対応記事: 「C言語で スケジュール管理アプリ（ミニ）を作ろう！」連載（予定）

ミニ日記アプリ（構造体・ファイルI/O）と祝日カレンダー企画（日付計算・祝日データ）の知識を組み合わせて実装する、日付付きスケジュール管理アプリのC言語版です。
まずは予定をメモリ上に追加・一覧表示するところから着手し、段階的にファイル保存・編集削除・日付ソート・祝日連携へと発展させる予定です。

```
c-language-studies/
└── schedule-mini-app/
    ├── ver1_add_and_list/        # メモリ上で追加・一覧表示
    │   └── schedule.c
    ├── ver2_save_and_load/       # ファイル保存・読み込み
    │   └── schedule.c
    ├── ver3_edit_and_delete/     # 編集・削除
    │   └── schedule.c
    ├── ver4_sort_by_date/        # 日付順ソート（バブルソート応用）
    │   └── schedule.c
    ├── ver5_holiday_link/        # 祝日データとの連携
    │   ├── schedule.c
    │   └── holidays_2026.csv
    ├── go/                       # Go版（Ver一括）予定
    │   └── schedule.go
    ├── kotlin/                   # Kotlin版（Ver一括）予定
    │   └── Schedule.kt
    └── README.md
```

## 予定している機能（Ver.1〜）

- Ver.1: 予定の追加・一覧表示（メモリ上）
- Ver.2: ファイルへの保存・読み込み
- Ver.3: 予定の編集・削除
- Ver.4: 日付順ソート（バブルソート応用、FE-algorithmと連動）
- Ver.5: 祝日データとの連携（`multi-pg-lang-calendar`のCSVフォーマットを流用予定）
- 将来的に他言語版（Go/Kotlin）とのデータフォーマット統一