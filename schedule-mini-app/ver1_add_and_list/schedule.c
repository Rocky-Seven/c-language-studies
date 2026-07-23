#include <stdio.h>
#include <string.h>

#define MAX_SCHEDULES 100
#define MAX_TITLE 100
#define MAX_DATE_LINE 64

/* 1件の予定を表す構造体。年・月・日と、予定の内容を保持する。 */
typedef struct {
    int year;
    int month;
    int day;
    char title[MAX_TITLE];
} Schedule;

/* 予定を保持する配列。Ver.1ではファイル保存は行わず、メモリ上にのみ保持する。 */
Schedule schedules[MAX_SCHEDULES];
int schedule_count = 0;

/* scanf直後に残った入力（改行など）を読み捨てる。 */
void clear_stdin(void) {
    int c;
    while ((c = getchar()) != '\n' && c != EOF);
}

void print_menu(void) {
    printf("\n=== スケジュール管理アプリ (Ver.1) ===\n");
    printf("1: 予定を追加\n");
    printf("2: 予定一覧を表示\n");
    printf("0: 終了\n");
    printf("番号を入力してください: ");
}

void add_schedule(void) {
    if (schedule_count >= MAX_SCHEDULES) {
        printf("これ以上予定を追加できません。\n");
        return;
    }

    Schedule *s = &schedules[schedule_count];
    char date_line[MAX_DATE_LINE];

    printf("年月日を入力してください（例: 2026 8 1 または 20260801）: ");

    /*
     * 年月日は1行分をfgetsでまとめて読み取り、sscanfで解析する。
     * scanf("%d %d %d", ...)を直接使うと、"20260801"のように
     * スペース無しでまとめて入力された場合に年しか読み取れず、
     * 月・日が未入力のまま次の入力（予定の内容のはず）とずれてしまう。
     *
     * そこで次の2通りの形式を順に試す。
     * 1) "2026 8 1" のような半角スペース区切り
     * 2) "20260801" のような8桁連続表記（年4桁 + 月2桁 + 日2桁）
     */
    fgets(date_line, sizeof(date_line), stdin);

    int result = sscanf(date_line, "%d %d %d", &s->year, &s->month, &s->day);
    if (result != 3) {
        result = sscanf(date_line, "%4d%2d%2d", &s->year, &s->month, &s->day);
    }

    /*
     * %dは先頭の"-"も符号として受け付けてしまうため、
     * "2026-08-01"のようなハイフン区切りを渡すと、
     * ハイフンを負の符号と誤認識して month=-8, day=-1 のように
     * 「解析には成功したが値がおかしい」状態になることがある。
     * そのため件数チェックに加えて、月日の値そのものが
     * 妥当な範囲かどうかも確認する。
     */
    if (result != 3 || s->month < 1 || s->month > 12 || s->day < 1 || s->day > 31) {
        printf("入力形式が正しくありません。「2026 8 1」または「20260801」の形式で入力してください。\n");
        return;
    }

    printf("予定を入力してください: ");
    fgets(s->title, MAX_TITLE, stdin);

    /* fgetsで読み込んだ文字列の末尾の改行を取り除く。 */
    s->title[strcspn(s->title, "\n")] = '\0';

    schedule_count++;
    printf("予定を追加しました。\n");
}

void list_schedules(void) {
    if (schedule_count == 0) {
        printf("登録されている予定はありません。\n");
        return;
    }

    printf("\n--- 予定一覧 ---\n");
    for (int i = 0; i < schedule_count; i++) {
        Schedule *s = &schedules[i];
        printf("%d: %04d-%02d-%02d  %s\n", i + 1, s->year, s->month, s->day, s->title);
    }
}

int main(void) {
    int choice;

    while (1) {
        print_menu();

        /* メニュー番号が数値以外の場合も、バッファを読み捨てて無限ループを防ぐ。 */
        if (scanf("%d", &choice) != 1) {
            clear_stdin();
            printf("正しい番号を入力してください。\n");
            continue;
        }
        clear_stdin();

        switch (choice) {
            case 1:
                add_schedule();
                break;
            case 2:
                list_schedules();
                break;
            case 0:
                printf("終了します。\n");
                return 0;
            default:
                printf("正しい番号を入力してください。\n");
        }
    }

    return 0;
}