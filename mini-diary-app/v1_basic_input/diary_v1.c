// ミニ日記アプリ ver.1（最低限の機能：入力と保存）
// 対応記事: 「C言語で ミニ日記アプリを作ろう！（1）」
// https://my-studies.org/lets-make-a-mini-diary-app-using-c-1/

#include <stdio.h>
#include <time.h>
#include <string.h>

int main(void) {
    char diary[1024];
    time_t now = time(NULL);
    struct tm *t = localtime(&now);
    char timestamp[100];
    strftime(timestamp, sizeof(timestamp), "%Y-%m-%d %H:%M:%S", t);

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
