#include <stdio.h>
#include <string.h>

#define MAX_SCHEDULES 100
#define MAX_TITLE 100
#define MAX_DATE_LINE 64
#define MAX_LINE 256
#define SCHEDULE_FILE "schedules.txt"

#define MAX_HOLIDAYS 1100
#define MAX_HOLIDAY_NAME 100
#define MAX_HOLIDAY_LINE 256
#define HOLIDAY_FILE "holidays.csv"

/* 1件の予定を表す構造体。年・月・日と、予定の内容を保持する。 */
typedef struct {
    int year;
    int month;
    int day;
    char title[MAX_TITLE];
} Schedule;

/* 1件の祝日を表す構造体。祝日カレンダー企画(multi-pg-lang-calendar)と同じ形式。 */
typedef struct {
    int year;
    int month;
    int day;
    char name[MAX_HOLIDAY_NAME];
} Holiday;

Schedule schedules[MAX_SCHEDULES];
int schedule_count = 0;

Holiday holidays[MAX_HOLIDAYS];
int holiday_count = 0;

/* scanf直後に残った入力（改行など）を読み捨てる。 */
void clear_stdin(void) {
    int c;
    while ((c = getchar()) != '\n' && c != EOF);
}

void print_menu(void) {
    printf("\n=== スケジュール管理アプリ (Ver.5) ===\n");
    printf("1: 予定を追加\n");
    printf("2: 予定一覧を表示\n");
    printf("3: 予定を編集\n");
    printf("4: 予定を削除\n");
    printf("5: 日付順に並び替え\n");
    printf("0: 終了\n");
    printf("番号を入力してください: ");
}

/*
 * 指定した年月日が祝日であれば、その名称を返す。祝日でなければNULLを返す。
 */
const char *find_holiday_name(int year, int month, int day) {
    for (int i = 0; i < holiday_count; i++) {
        if (holidays[i].year == year && holidays[i].month == month && holidays[i].day == day) {
            return holidays[i].name;
        }
    }
    return NULL;
}

/*
 * 祝日データ(holidays.csv)を読み込む。
 * 「マルチ言語で祝日対応のカレンダーをつくろう！」企画(multi-pg-lang-calendar)で
 * 使っているものと同じ形式（内閣府CSV由来、1行目はヘッダー）を想定している。
 * ファイルが無い場合は、祝日連携なしで続行する（エラー扱いにはしない）。
 */
void load_holidays(void) {
    FILE *fp = fopen(HOLIDAY_FILE, "r");
    if (fp == NULL) {
        printf("祝日データ(%s)が見つかりませんでした。祝日連携なしで続行します。\n", HOLIDAY_FILE);
        return;
    }

    char line[MAX_HOLIDAY_LINE];

    /* 1行目はヘッダー行（項目名）のため読み飛ばす。 */
    fgets(line, sizeof(line), fp);

    while (fgets(line, sizeof(line), fp) != NULL && holiday_count < MAX_HOLIDAYS) {
        line[strcspn(line, "\n")] = '\0';
        line[strcspn(line, "\r")] = '\0';

        char *comma = strchr(line, ',');
        if (comma == NULL) {
            continue;
        }

        *comma = '\0';
        char *date_part = line;
        char *name_part = comma + 1;

        int year, month, day;

        /* 内閣府CSVは"年/月/日"形式。念のため"年-月-日"にも対応しておく。 */
        int result = sscanf(date_part, "%d/%d/%d", &year, &month, &day);
        if (result != 3) {
            result = sscanf(date_part, "%d-%d-%d", &year, &month, &day);
        }

        if (result != 3 || month < 1 || month > 12 || day < 1 || day > 31) {
            continue;
        }

        Holiday *h = &holidays[holiday_count];
        h->year = year;
        h->month = month;
        h->day = day;
        strncpy(h->name, name_part, MAX_HOLIDAY_NAME - 1);
        h->name[MAX_HOLIDAY_NAME - 1] = '\0';
        holiday_count++;
    }

    fclose(fp);
    printf("祝日データを読み込みました: %d件\n", holiday_count);
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
     * （2月31日のような実在しない日付までは確認していない）
     */
    if (result != 3 || s->month < 1 || s->month > 12 || s->day < 1 || s->day > 31) {
        printf("入力形式が正しくありません。「2026 8 1」または「20260801」の形式で入力してください。\n");
        return 0;
    }

    /* 祝日と重なる場合は、参考情報として知らせる（入力を止めるものではない）。 */
    const char *holiday_name = find_holiday_name(s->year, s->month, s->day);
    if (holiday_name != NULL) {
        printf("この日は祝日です（%s）。\n", holiday_name);
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

/* 祝日と重なる予定には、一覧表示の末尾に祝日名を添える。 */
void list_schedules(void) {
    if (schedule_count == 0) {
        printf("登録されている予定はありません。\n");
        return;
    }

    printf("\n--- 予定一覧 ---\n");
    for (int i = 0; i < schedule_count; i++) {
        Schedule *s = &schedules[i];
        const char *holiday_name = find_holiday_name(s->year, s->month, s->day);

        if (holiday_name != NULL) {
            printf("%d: %04d-%02d-%02d  %s  [祝: %s]\n", i + 1, s->year, s->month, s->day, s->title, holiday_name);
        } else {
            printf("%d: %04d-%02d-%02d  %s\n", i + 1, s->year, s->month, s->day, s->title);
        }
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

/* aの日付がbより後ろ（大きい）であれば1を返す。年→月→日の順で比較する。 */
int is_later(const Schedule *a, const Schedule *b) {
    if (a->year != b->year) {
        return a->year > b->year;
    }
    if (a->month != b->month) {
        return a->month > b->month;
    }
    return a->day > b->day;
}

/*
 * 日付（年→月→日の順）を基準に、予定を昇順に並び替える。
 * 基本情報技術者試験アルゴリズムシリーズのバブルソートと同じ考え方を、
 * Schedule構造体の配列に対して適用したもの。
 */
void sort_schedules_by_date(void) {
    if (schedule_count < 2) {
        printf("並び替えの対象となる予定がありません。\n");
        return;
    }

    for (int i = 0; i < schedule_count - 1; i++) {
        int swapped = 0;

        for (int j = 0; j < schedule_count - 1 - i; j++) {
            if (is_later(&schedules[j], &schedules[j + 1])) {
                Schedule temp = schedules[j];
                schedules[j] = schedules[j + 1];
                schedules[j + 1] = temp;
                swapped = 1;
            }
        }

        if (!swapped) {
            break;
        }
    }

    printf("日付順に並び替えました。\n");
    list_schedules();
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

    load_holidays();
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
            case 5:
                sort_schedules_by_date();
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
