#define _POSIX_C_SOURCE 200809L  // setenv, tzset などの宣言を可視化（警告対策）

// diary_ver6.1.c - UTF-8対応・100文字制限・部分採用＋超過通知・マルチバイト文字数カウント・ロケール堅牢化版
// 対応記事: 「C言語で ミニ日記アプリを作ろう！（6.1）：Ver.6からVer.6.1へ「部分採用＋通知」方式で挙動を改善」
// https://my-studies.org/lets-make-a-mini-diary-app-using-c-06-01/
//
// Ver.6からの変更点：上限超過時に「切り捨てた文字数」を明示する通知を追加（UIの分かりやすさ改善）

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <locale.h>
#include <time.h>
#include <wchar.h>
#include <wctype.h>

#if defined(__linux__) || defined(__APPLE__)
  #include <langinfo.h>
  #define HAVE_LANGINFO 1
#endif

#define MAX_ENTRY   1000
#define MAX_LINE    1024
#define CONTENT_MAX (MAX_LINE * 10)

typedef struct {
    char content[CONTENT_MAX];
    char month[16];  // "YYYY-MM" または "未記録"
} DiaryEntry;

/* UTF-8 ロケールを確実に使うための初期化 */
static void init_locale_utf8(void) {
    const char *ok = NULL;

    // 環境のロケール（LANG/LC_*）を優先
    ok = setlocale(LC_ALL, "");
    // 代表的なUTF-8ロケールにフェイルオーバー
    if (!ok) ok = setlocale(LC_ALL, "ja_JP.UTF-8");
    if (!ok) ok = setlocale(LC_ALL, "C.UTF-8");
    if (!ok) ok = setlocale(LC_ALL, "en_US.UTF-8");

#if defined(HAVE_LANGINFO)
    if (ok) {
        const char *cs = nl_langinfo(CODESET);
        if (!cs || (strcmp(cs, "UTF-8") != 0 && strcmp(cs, "UTF8") != 0)) {
            fprintf(stderr,
                "警告: 現在のロケールはUTF-8ではありません（CODESET=%s）。"
                " 日本語の文字数カウントが正しく動作しない可能性があります。\n",
                cs ? cs : "(不明)");
        }
    } else {
        fprintf(stderr,
            "警告: UTF-8ロケールの設定に失敗しました。"
            " OS に ja_JP.UTF-8 / C.UTF-8 などを導入してください。\n");
    }
#else
    if (!ok) {
        fprintf(stderr,
            "警告: ロケールの設定に失敗しました（UTF-8未設定）。\n");
    }
#endif
}

/* マルチバイト文字を1文字としてカウント（改行除外） */
static size_t count_characters(const char *s) {
    size_t count = 0;
    wchar_t wc;
    mbstate_t state;
    memset(&state, 0, sizeof(state));

    while (*s) {
        size_t len = mbrtowc(&wc, s, MB_CUR_MAX, &state);
        if (len == (size_t)-1) { // 不正シーケンス：1バイト進めて継続
            ++s;
            memset(&state, 0, sizeof(state));
            continue;
        }
        if (len == (size_t)-2) { // 不完全（末尾）
            break;
        }
        if (len == 0) { // 終端
            break;
        }
        if (wc != L'\n' && wc != L'\r') count++;
        s += len;
    }
    return count;
}

/* 空行（空白/タブ/CRのみ + LF or 終端）を判定
   ※全角スペース(U+3000)は空白扱いにしていない点に注意 */
static int is_blank_line(const char *s) {
    const unsigned char *p = (const unsigned char *)s;
    while (*p == ' ' || *p == '\t' || *p == '\r') p++;
    return (*p == '\n' || *p == '\0');
}

static void make_timestamp(char *buf, size_t bufsize) {
    time_t now = time(NULL);
    struct tm *t = localtime(&now);
    const char *weekdays[] = {"日","月","火","水","木","金","土"};
    snprintf(buf, bufsize, "%04d-%02d-%02d（%s） %02d:%02d:%02d",
             t->tm_year + 1900, t->tm_mon + 1, t->tm_mday,
             weekdays[t->tm_wday], t->tm_hour, t->tm_min, t->tm_sec);
}

/* 先頭の [YYYY-MM-...] から YYYY-MM を安全に抽出 */
static void extract_month(const char *entry_text, char out[16]) {
    const char *start = strchr(entry_text, '[');
    if (!start) { strcpy(out, "未記録"); return; }

    char y[5] = {0}, m[3] = {0};
    if (sscanf(start + 1, "%4[0-9]-%2[0-9]", y, m) == 2) {
        snprintf(out, 16, "%s-%s", y, m);
    } else {
        strcpy(out, "未記録");
    }
}

int read_diary_entries(const char *filename, DiaryEntry *entries, int max_entries) {
    FILE *fp = fopen(filename, "r");
    if (!fp) return 0;

    char line[MAX_LINE];
    char buffer[CONTENT_MAX]; buffer[0] = '\0';
    int entry_count = 0;

    while (fgets(line, sizeof(line), fp)) {
        if (is_blank_line(line)) {
            if (buffer[0] != '\0' && entry_count < max_entries) {
                strncpy(entries[entry_count].content, buffer, CONTENT_MAX - 1);
                entries[entry_count].content[CONTENT_MAX - 1] = '\0';
                extract_month(buffer, entries[entry_count].month);
                entry_count++;
                buffer[0] = '\0';
            }
        } else {
            size_t cur = strlen(buffer);
            size_t rem = (CONTENT_MAX - 1) - cur;
            if (rem > 0) strncat(buffer, line, rem);
        }
    }

    if (buffer[0] != '\0' && entry_count < max_entries) {
        strncpy(entries[entry_count].content, buffer, CONTENT_MAX - 1);
        entries[entry_count].content[CONTENT_MAX - 1] = '\0';
        extract_month(buffer, entries[entry_count].month);
        entry_count++;
    }

    fclose(fp);
    return entry_count;
}

static void show_menu(void) {
    printf("\n********** MENU **********\n");
    printf("1. 過去2件の日記を表示する\n");
    printf("2. 新しい日記を書く（複数行）\n");
    printf("3. 月ごとの一覧を表示する\n");
    printf("9. 終了する\n");
    printf("***************************\n");
    printf("選択肢を入力してください：");
}

static void show_latest_two_entries(void) {
    DiaryEntry *entries = (DiaryEntry *)calloc(MAX_ENTRY, sizeof(DiaryEntry));
    if (!entries) { puts("メモリ確保に失敗しました。"); return; }

    int count = read_diary_entries("diary.txt", entries, MAX_ENTRY);

    printf("\n=== 過去2件（日付が新しい順） ===\n");
    if (count <= 0) {
        printf("（過去の日記はありません）\n");
        free(entries);
        return;
    }

    int start = (count >= 2) ? count - 2 : 0;
    for (int i = count - 1; i >= start; --i) {
        printf("%s\n", entries[i].content);
        if (i > start) puts("--------------------------------");
    }
    free(entries);
}

static void show_monthly_summary(void) {
    DiaryEntry *entries = (DiaryEntry *)calloc(MAX_ENTRY, sizeof(DiaryEntry));
    if (!entries) { puts("メモリ確保に失敗しました。"); return; }

    int count = read_diary_entries("diary.txt", entries, MAX_ENTRY);
    if (count <= 0) {
        printf("\n（日記がありません）\n");
        free(entries);
        return;
    }

    printf("\n=== 月ごとの一覧（古い→新しい） ===\n");
    char current_month[16] = "";
    for (int i = 0; i < count; i++) {
        if (strcmp(current_month, entries[i].month) != 0) {
            strncpy(current_month, entries[i].month, sizeof(current_month)-1);
            current_month[sizeof(current_month)-1] = '\0';
            printf("\n■ %s の日記:\n", current_month[0] ? current_month : "未記録");
        }
        printf("%s\n", entries[i].content);
    }
    free(entries);
}

static void write_new_entry(void) {
    char diary[CONTENT_MAX] = "";
    char line[MAX_LINE];

    printf("\n=== 今日の日記を入力してください（空行で終了） ===\n");
    while (fgets(line, sizeof(line), stdin)) {
        if (is_blank_line(line)) break;

        size_t cur_len = count_characters(diary);
        if (cur_len >= 100) {
            puts("※ すでに100文字に達したため、これ以上は記録されません。");
            break;
        }

        // この行が持つ「非改行の文字数」を先に数え、超過数を算出
        size_t line_chars = count_characters(line);                    // 改行は数えない
        size_t discard = 0;
        if (cur_len + line_chars > 100) {
            discard = (cur_len + line_chars) - 100;                    // この行で捨てられる「文字数」
        }

        size_t remaining = 100 - cur_len;                              // 取り込める残りの「文字」数
        size_t copy_len = 0;                                           // 取り込む「バイト」数
        wchar_t wc;
        mbstate_t state;
        memset(&state, 0, sizeof(state));
        const char *p = line;

        // UTF-8 文字単位で、remaining が尽きるまでバイトを積む
        while (*p && remaining > 0) {
            size_t len = mbrtowc(&wc, p, MB_CUR_MAX, &state);
            if (len == (size_t)-1) {               // 不正シーケンス：1バイト飛ばして継続
                ++p;
                memset(&state, 0, sizeof(state));
                continue;
            }
            if (len == (size_t)-2 || len == 0) {   // 不完全/終端
                break;
            }
            if (wc != L'\n' && wc != L'\r') {
                remaining--;
            }
            copy_len += len;
            p += len;
        }

        // 許容分だけ追記（改行は line に含まれていても count_characters が除外済み）
        strncat(diary, line, copy_len);

        // 現在の採用後文字数を表示
        size_t now = count_characters(diary);
        printf("（現在 %zu 文字 / 最大100文字）\n", now);

        // 上限超過による末尾切り捨てが発生していたら通知
        if (discard > 0) {
            printf("※ 100文字の上限により、この行の末尾 %zu 文字は保存されませんでした。\n", discard);
        }
    }

    size_t len = count_characters(diary);
    if (len < 20) {
        puts("内容が短すぎます。20文字以上で入力してください。");
        return;
    }

    char timestamp[128];
    make_timestamp(timestamp, sizeof(timestamp));

    FILE *fp = fopen("diary.txt", "a");
    if (!fp) {
        perror("ファイルを開けませんでした");
        return;
    }

    fprintf(fp, "[%s]\n%s\n\n", timestamp, diary);
    fclose(fp);
    puts("日記を保存しました。");
}

int main(void) {
    setenv("TZ", "Asia/Tokyo", 1);
    tzset();
    init_locale_utf8();  // UTF-8 ロケールを確保

    int choice;
    for (;;) {
        show_menu();

        if (scanf("%d", &choice) != 1) {
            puts("数字を入力してください。");
            int ch; while ((ch = getchar()) != '\n' && ch != EOF) {}
            continue;
        }
        int ch; while ((ch = getchar()) != '\n' && ch != EOF) {}

        switch (choice) {
            case 1: show_latest_two_entries(); break;
            case 2: write_new_entry(); break;
            case 3: show_monthly_summary(); break;
            case 9: puts("終了します。"); return 0;
            default: puts("無効な選択です。もう一度入力してください。");
        }
    }
    return 0;
}
