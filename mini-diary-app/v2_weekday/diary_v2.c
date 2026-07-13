// ミニ日記アプリ ver.2（曜日付き）
// 対応記事: 「C言語で ミニ日記アプリを作ろう！（2）」
// https://my-studies.org/lets-make-a-mini-diary-app-using-c-02/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <locale.h>
#include <time.h>

#define MAX_INPUT 1024

int main(void) {
    // ロケールをUTF-8に設定（MSYS2/UCRT64 なら ja_JP.UTF-8 が有効）
    setlocale(LC_ALL, "ja_JP.UTF-8");

    char diary[MAX_INPUT];

    time_t now = time(NULL);
    struct tm *t = localtime(&now);

    // 曜日を取得（日本語）
    const char *weekdays_jp[] = {"日", "月", "火", "水", "木", "金", "土"};

    char timestamp[128];

    // 曜日（日本語）を含めた表示が可能
    snprintf(timestamp, sizeof(timestamp),
             "%04d-%02d-%02d（%s） %02d:%02d:%02d",
             t->tm_year + 1900, t->tm_mon + 1, t->tm_mday,
             weekdays_jp[t->tm_wday],
             t->tm_hour, t->tm_min, t->tm_sec);

    // 入力受付（入力は１行のみ）
    printf("今日の日記を入力してください（Enterで終了）：\n");
    if (fgets(diary, sizeof(diary), stdin) == NULL) {
        fprintf(stderr, "入力に失敗しました。\n");
        return 1;
    }

    // 改行を取り除く
    size_t len = strlen(diary);
    if (len > 0 && diary[len - 1] == '\n') {
        diary[len - 1] = '\0';
    }

    // 空文字列ならスキップ
    if (strlen(diary) == 0) {
        printf("入力が空です。保存しません。\n");
        return 0;
    }

    // UTF-8で追記（BOMなし）
    FILE *fp = fopen("diary.txt", "a");
    if (fp == NULL) {
        perror("ファイルを開けませんでした");
        return 1;
    }

    fprintf(fp, "[%s]\n%s\n\n", timestamp, diary);
    fclose(fp);

    printf("日記を保存しました。\n");
    return 0;
}
