// ミニ日記アプリ ver.4.1（Ver.4 の「タイムスタンプ二重カッコ」バグを修正）
//
// 対応記事: 「C言語で ミニ日記アプリを作ろう！（4）」の派生（Ver.4.1）
// https://my-studies.org/lets-make-a-mini-diary-app-using-c-04/
//
// 修正点（Ver.4 → Ver.4.1）：
//   load_entries() でタイムスタンプ行 "[YYYY-MM-DD（曜） HH:MM:SS]" を読み込む際、
//   先頭の "[" は除去していたが、末尾の "]" を除去していなかった。
//   このため、表示時に "[2026-07-13（月） 06:07:42]]" のように
//   閉じ括弧が二重になっていた（Ver.3〜Ver.4 に共通のバグ）。
//   → 改行除去の直後に、末尾が ']' なら取り除く処理を追加した。

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <locale.h>
#include <time.h>

#define MAX_INPUT 1024
#define MAX_ENTRIES 1000

typedef struct {
    char timestamp[128];
    char content[MAX_INPUT];
} DiaryEntry;

// --- メニュー表示 ---
void show_menu(void) {
    printf("\n********** MENU **********\n");
    printf("1. 過去2件の日記を表示する\n");
    printf("2. 新しい日記を書く\n");
    printf("9. 終了する\n");
    printf("***************************\n");
    printf("選択肢を入力してください：");
}

// --- ファイル読み込み ---
int load_entries(const char *filename, DiaryEntry entries[], int max_entries) {
    FILE *fp = fopen(filename, "r");
    if (!fp) {
        return 0;
    }

    char line[MAX_INPUT];
    int count = 0;
    DiaryEntry temp = {"", ""};

    while (fgets(line, sizeof(line), fp)) {
        if (line[0] == '[') {
            if (strlen(temp.timestamp) > 0) {
                if (count < max_entries) {
                    entries[count++] = temp;
                }
                temp.timestamp[0] = '\0';
                temp.content[0] = '\0';
            }

            // 改行を除去
            line[strcspn(line, "\n")] = '\0';

            // ★修正点：末尾の ']' を除去（Ver.4では未対応だった）
            size_t llen = strlen(line);
            if (llen > 0 && line[llen - 1] == ']') {
                line[llen - 1] = '\0';
            }

            strncpy(temp.timestamp, line + 1, sizeof(temp.timestamp) - 1); // 先頭の [ を除く
        } else if (strlen(temp.timestamp) > 0 && line[0] != '\n') {
            line[strcspn(line, "\n")] = '\0';
            strncpy(temp.content, line, sizeof(temp.content) - 1);
        }
    }

    if (strlen(temp.timestamp) > 0) {
        if (count < max_entries) {
            entries[count++] = temp;
        }
    }

    fclose(fp);
    return count;
}

// --- 過去2件表示 ---
void show_last_two_entries(void) {
    DiaryEntry entries[MAX_ENTRIES];
    int entry_count = load_entries("diary.txt", entries, MAX_ENTRIES);

    printf("\n【過去2件の記録】\n");
    if (entry_count == 0) {
        printf("なし\n");
        return;
    }
    int start = (entry_count >= 2) ? entry_count - 2 : 0;
    for (int i = start; i < entry_count; i++) {
        printf("[%s]\n%s\n\n", entries[i].timestamp, entries[i].content);
    }
}

// --- 新規日記作成 ---
void write_new_entry(void) {
    char diary[MAX_INPUT];
    time_t now = time(NULL);
    struct tm *t = localtime(&now);
    const char *weekdays_jp[] = {"日", "月", "火", "水", "木", "金", "土"};

    char timestamp[128];
    snprintf(timestamp, sizeof(timestamp),
             "%04d-%02d-%02d（%s） %02d:%02d:%02d",
             t->tm_year + 1900, t->tm_mon + 1, t->tm_mday,
             weekdays_jp[t->tm_wday],
             t->tm_hour, t->tm_min, t->tm_sec);

    printf("今日の日記を入力してください（Enterで終了）：\n");
    if (!fgets(diary, sizeof(diary), stdin)) {
        fprintf(stderr, "入力に失敗しました。\n");
        return;
    }
    size_t len = strlen(diary);
    if (len > 0 && diary[len - 1] == '\n') {
        diary[len - 1] = '\0';
    }

    if (strlen(diary) == 0) {
        printf("入力が空です。保存しません。\n");
        return;
    }

    FILE *fp = fopen("diary.txt", "a");
    if (!fp) {
        perror("ファイルを開けませんでした");
        return;
    }
    fprintf(fp, "[%s]\n%s\n\n", timestamp, diary);
    fclose(fp);

    printf("日記を保存しました。\n");
}

// --- メイン ---
int main(void) {
    setlocale(LC_ALL, "ja_JP.UTF-8");

    while (1) {
        show_menu();

        char choice[10];
        if (!fgets(choice, sizeof(choice), stdin)) {
            printf("入力エラーです。\n");
            continue;
        }

        int opt = atoi(choice);

        switch (opt) {
            case 1:
                show_last_two_entries();
                break;
            case 2:
                write_new_entry();
                break;
            case 9:
                printf("終了します。\n");
                return 0;
            default:
                printf("無効な選択肢です。\n");
        }
    }
}
