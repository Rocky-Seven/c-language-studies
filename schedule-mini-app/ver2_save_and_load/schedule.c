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

/* 予定を保持する配列。Ver.2ではプログラム終了時にファイルへ保存し、次回起動時に読み込む。 */
Schedule schedules[MAX_SCHEDULES];
int schedule_count = 0;

/* scanf直後に残った入力（改行など）を読み捨てる。 */
void clear_stdin(void) {
    int c;
    while ((c = getchar()) != '\n' && c != EOF);
}

void print_menu(void) {
    printf("\n=== スケジュール管理アプリ (Ver.2) ===\n");
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

    /*
     * 予定が空文字だと、ファイル保存時に「年,月,日,」という行になり、
     * 読み込み時に%[^\n]が0文字にはマッチできず、
     * この1件だけが静かに消えてしまう問題がある。
     * そのため、保存の仕組みを複雑にするのではなく、
     * 入力の時点で空の予定を弾く。
     */
    if (s->title[0] == '\0') {
        printf("予定の内容が入力されていません。追加を中止します。\n");
        return;
    }

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

        /*
         * 保存形式「年,月,日,予定」に対応するsscanf。
         * 予定の部分は改行の手前まで（%[^\n]）読み取ることで、
         * 予定の内容に読点や空白が含まれていても正しく読み込める。
         * 予定が空文字の行（データが壊れている場合など）は読み取りに失敗するため、
         * その行はスキップする。
         */
        int result = sscanf(line, "%d,%d,%d,%99[^\n]", &s->year, &s->month, &s->day, s->title);

        /*
         * add_schedule()と同様に、月日の範囲もあわせて確認する。
         * ファイルを手動で編集した場合などに、壊れた日付
         * （例: 月が13、日が40など）が紛れ込む可能性があるため。
         */
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
                save_schedules();
                printf("終了します。\n");
                return 0;
            default:
                printf("正しい番号を入力してください。\n");
        }
    }

    return 0;
}
