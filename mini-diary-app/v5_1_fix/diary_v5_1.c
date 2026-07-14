// ミニ日記アプリ ver.5.1（GitHub内のみの整備版）
//
// ベース: Ver.5（メニュー＋月ごとの一覧付き）
// https://my-studies.org/lets-make-a-mini-diary-app-using-c-05/
//
// ※本バージョンに対応するブログ記事はありません。
//   ブログ公開（2025.08.11）から約1年が経過していることもあり、
//   Ver.4.1 で行った修正内容（タイムスタンプ二重カッコの解消）と、
//   Ver.6 で導入されたタイムゾーン明示指定（Asia/Tokyo）を、
//   GitHubリポジトリ内の所要の整備として反映したもの。
//
// Ver.5 → Ver.5.1 の変更点：
//   (1) load_entries() のタイムスタンプ二重カッコ（]]）バグを修正（Ver.4.1と同内容）
//   (2) main() 冒頭で setenv("TZ", "Asia/Tokyo", 1); tzset(); を追加し、
//       実行環境（GitHub Codespaces等、既定でUTCになりがちな環境）に依存せず
//       常に日本時間（JST）でタイムスタンプを記録するようにした（Ver.6と同内容）

#define _POSIX_C_SOURCE 200809L  // setenv, tzset の宣言を可視化（警告対策）

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
    printf("3. 月ごとの一覧を表示する\n");
    printf("9. 終了する\n");
    printf("***************************\n");
    printf("選択した番号を入力してください：");
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

            // ★修正点（Ver.4.1と同内容）：末尾の ']' を除去
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

// --- 月ごとの一覧表示 ---
void show_monthly_list(void) {
    DiaryEntry entries[MAX_ENTRIES];
    int entry_count = load_entries("diary.txt", entries, MAX_ENTRIES);

    if (entry_count == 0) {
        printf("\n記録はありません。\n");
        return;
    }

    printf("\n【月ごとの一覧】\n");

    char current_month[8] = "";
    for (int i = 0; i < entry_count; i++) {
        char month[8];
        strncpy(month, entries[i].timestamp, 7); // "YYYY-MM"
        month[7] = '\0';

        if (strcmp(current_month, month) != 0) {
            strcpy(current_month, month);
            printf("\n=== %s ===\n", current_month);
        }
        printf("[%s] %s\n", entries[i].timestamp, entries[i].content);
    }
}

// --- メイン ---
int main(void) {
    // ★修正点（Ver.6と同内容）：実行環境のタイムゾーンに依存せず、常にJSTでタイムスタンプを生成する
    setenv("TZ", "Asia/Tokyo", 1);
    tzset();

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
            case 3:
                show_monthly_list();
                break;
            case 9:
                printf("終了します。\n");
                return 0;
            default:
                printf("無効な選択肢です。\n");
        }
    }
}
