#include <stdio.h>
#include <string.h>

#define MAX_SCHEDULES 100
#define MAX_TITLE 100
#define MAX_DATE_LINE 64
#define MAX_LINE 256
#define SCHEDULE_FILE "schedules.txt"

/* 1件の予定を表す構造体。年・月・日と、予定の内容を保持する。 */
typedef struct {
    int year;
    int month;
    int day;
    char title[MAX_TITLE];
} Schedule;

/* 予定を保持する配列。 */
Schedule schedules[MAX_SCHEDULES];
int schedule_count = 0;

/* scanf直後に残った入力（改行など）を読み捨てる。 */
void clear_stdin(void) {
    int c;
    while ((c = getchar()) != '\n' && c != EOF);
}

void print_menu(void) {
    printf("\n=== スケジュール管理アプリ (Ver.3) ===\n");
    printf("1: 予定を追加\n");
    printf("2: 予定一覧を表示\n");
    printf("3: 予定を編集\n");
    printf("4: 予定を削除\n");
    printf("0: 終了\n");
    printf("番号を入力してください: ");
}

/*
 * 年月日と予定の内容を標準入力から読み取り、sに格納する。
 * 追加(add_schedule)・編集(edit_schedule)の両方から共通で使う。
 * 読み取りに成功すれば1、形式が不正であれば0を返す。
 */
int read_schedule_input(Schedule *s) {
    char date_line[MAX_DATE_LINE];

    printf("年月日を入力してください（例: 2026 8 1 または 20260801）: ");

    /*
     * 年月日は1行分をfgetsでまとめて読み取り、sscanfで解析する。
     * 対応する形式は次の2通り。
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
     * "2026-08-01"のようなハイフン区切りは、ハイフンを負の符号と
     * 誤認識して「解析には成功したが値がおかしい」状態になる。
     * 件数チェックに加えて、月日の値が妥当な範囲かも確認する。
     */
    if (result != 3 || s->month < 1 || s->month > 12 || s->day < 1 || s->day > 31) {
        printf("入力形式が正しくありません。「2026 8 1」または「20260801」の形式で入力してください。\n");
        return 0;
    }

    printf("予定を入力してください: ");
    fgets(s->title, MAX_TITLE, stdin);
    s->title[strcspn(s->title, "\n")] = '\0';

    /*
     * 予定が空文字だと、ファイル保存時に"年,月,日,"という行になり、
     * 読み込み時に静かに消えてしまう問題があるため、
     * 入力の時点で空の予定を弾く。
     */
    if (s->title[0] == '\0') {
        printf("予定の内容が入力されていません。処理を中止します。\n");
        return 0;
    }

    return 1;
}

void add_schedule(void) {
    if (schedule_count >= MAX_SCHEDULES) {
        printf("これ以上予定を追加できません。\n");
        return;
    }

    Schedule temp;
    if (!read_schedule_input(&temp)) {
        return;
    }

    schedules[schedule_count] = temp;
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

/*
 * 一覧表示したうえで番号を入力してもらい、対応する配列の添字（0始まり）を返す。
 * 予定が1件も無い場合、数値でない場合、範囲外の番号の場合は-1を返す。
 * edit_schedule・delete_scheduleの両方から共通で使う。
 */
int select_schedule_index(const char *prompt) {
    if (schedule_count == 0) {
        printf("登録されている予定はありません。\n");
        return -1;
    }

    list_schedules();

    printf("%s", prompt);
    int number;
    if (scanf("%d", &number) != 1) {
        clear_stdin();
        printf("正しい番号を入力してください。\n");
        return -1;
    }
    clear_stdin();

    if (number < 1 || number > schedule_count) {
        printf("該当する予定がありません。\n");
        return -1;
    }

    return number - 1;
}

void edit_schedule(void) {
    int index = select_schedule_index("編集する予定の番号を入力してください: ");
    if (index < 0) {
        return;
    }

    Schedule temp;
    if (!read_schedule_input(&temp)) {
        printf("編集を中止しました。\n");
        return;
    }

    schedules[index] = temp;
    printf("予定を更新しました。\n");
}

void delete_schedule(void) {
    int index = select_schedule_index("削除する予定の番号を入力してください: ");
    if (index < 0) {
        return;
    }

    /* 削除した分だけ、後ろの要素を1つずつ前に詰める。 */
    for (int i = index; i < schedule_count - 1; i++) {
        schedules[i] = schedules[i + 1];
    }
    schedule_count--;

    printf("予定を削除しました。\n");
}

/* 現在保持している予定をすべてファイルに書き出す。1件につき1行、"年,月,日,予定"の形式。 */
void save_schedules(void) {
    FILE *fp = fopen(SCHEDULE_FILE, "w");
    if (fp == NULL) {
        printf("ファイルへの保存に失敗しました。\n");
        return;
    }

    for (int i = 0; i < schedule_count; i++) {
        Schedule *s = &schedules[i];
        fprintf(fp, "%d,%d,%d,%s\n", s->year, s->month, s->day, s->title);
    }

    fclose(fp);
}

/*
 * ファイルから予定を読み込み、現在メモリ上にある内容を置き換える。
 * ファイルが存在しない場合（初回起動時など）は、何もせず空の状態で始める。
 */
void load_schedules(void) {
    FILE *fp = fopen(SCHEDULE_FILE, "r");
    if (fp == NULL) {
        return;
    }

    schedule_count = 0;
    int skipped = 0;
    char line[MAX_LINE];

    while (fgets(line, sizeof(line), fp) != NULL) {
        if (schedule_count >= MAX_SCHEDULES) {
            skipped++;
            continue;
        }

        Schedule *s = &schedules[schedule_count];

        int result = sscanf(line, "%d,%d,%d,%99[^\n]", &s->year, &s->month, &s->day, s->title);

        if (result == 4 && s->month >= 1 && s->month <= 12 && s->day >= 1 && s->day <= 31) {
            schedule_count++;
        } else {
            skipped++;
        }
    }

    fclose(fp);

    if (skipped > 0) {
        printf("注意: 読み込めなかったデータが%d件ありました。\n", skipped);
    }
}

int main(void) {
    int choice;

    load_schedules();

    while (1) {
        print_menu();

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
            case 3:
                edit_schedule();
                break;
            case 4:
                delete_schedule();
                break;
            case 0:
                save_schedules();
                printf("終了します。\n");
                return 0;
            default:
                printf("正しい番号を入力してください。\n");
        }
    }

    return 0;
}
