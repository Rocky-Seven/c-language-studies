// ミニ日記アプリ ver.3（曜日付き＋過去2件表示）
// 対応記事: 「C言語で ミニ日記アプリを作ろう！（3）」
// https://my-studies.org/lets-make-a-mini-diary-app-using-c-03/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <locale.h>
#include <time.h>

#define MAX_INPUT 1024
#define MAX_ENTRIES 1000   // 読み込み可能な最大エントリ数

// 過去エントリ構造体
typedef struct {
    char timestamp[128];
    char content[MAX_INPUT];
} DiaryEntry;

// diary.txtを読み込み、エントリ数を返す
int load_entries(const char *filename, DiaryEntry entries[], int max_entries) {
    FILE *fp = fopen(filename, "r");
    if (!fp) {
        return 0; // ファイルなし
    }

    char line[MAX_INPUT];
    int count = 0;
    DiaryEntry temp = {"", ""};

    while (fgets(line, sizeof(line), fp)) {
        // タイムスタンプ行（[YYYY-... で始まる）
        if (line[0] == '[') {
            if (strlen(temp.timestamp) > 0) {
                // 直前のエントリを保存
                if (count < max_entries) {
                    entries[count++] = temp;
                }
                temp.timestamp[0] = '\0';
                temp.content[0] = '\0';
            }
            line[strcspn(line, "\n")] = '\0';
            strncpy(temp.timestamp, line + 1, sizeof(temp.timestamp) - 1); // [ を除く
        }
        // 内容行
        else if (strlen(temp.timestamp) > 0 && line[0] != '\n') {
            line[strcspn(line, "\n")] = '\0';
            strncpy(temp.content, line, sizeof(temp.content) - 1);
        }
    }

    // 最後のエントリ保存
    if (strlen(temp.timestamp) > 0) {
        if (count < max_entries) {
            entries[count++] = temp;
        }
    }

    fclose(fp);
    return count;
}

int main(void) {
    setlocale(LC_ALL, "ja_JP.UTF-8");

    DiaryEntry entries[MAX_ENTRIES];
    int entry_count = load_entries("diary.txt", entries, MAX_ENTRIES);

    // --- 過去2件の表示 ---
    printf("【過去2件の記録】\n");
    if (entry_count == 0) {
        printf("なし\n\n");
    } else {
        int start = (entry_count >= 2) ? entry_count - 2 : 0;
        for (int i = start; i < entry_count; i++) {
            printf("[%s]\n%s\n\n", entries[i].timestamp, entries[i].content);
        }
    }

    // --- 新規入力 ---
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
        return 1;
    }

    size_t len = strlen(diary);
    if (len > 0 && diary[len - 1] == '\n') {
        diary[len - 1] = '\0';
    }

    if (strlen(diary) == 0) {
        printf("入力が空です。保存しません。\n");
        return 0;
    }

    FILE *fp = fopen("diary.txt", "a");
    if (!fp) {
        perror("ファイルを開けませんでした");
        return 1;
    }
    fprintf(fp, "[%s]\n%s\n\n", timestamp, diary);
    fclose(fp);

    printf("日記を保存しました。\n");
    return 0;
}
