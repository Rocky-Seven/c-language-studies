#include <stdio.h>

#define SIZE 10

/* 配列の中身を表示する関数 */
void print_array(int arr[], int size) {
    for (int i = 0; i < size; i++) {
        printf("%d ", arr[i]);
    }
    printf("\n");
}

/*
 * 線形探索
 * 先頭から順番に1つずつ比較していく。
 * 戻り値: 見つかった位置（見つからなければ -1）
 */
int linear_search(int arr[], int size, int target) {
    int comparisons = 0;

    for (int i = 0; i < size; i++) {
        comparisons++;
        printf("  比較%d回目: arr[%d] = %d と %d を比較\n", comparisons, i, arr[i], target);

        if (arr[i] == target) {
            printf("  → 見つかりました（比較回数: %d回）\n", comparisons);
            return i;
        }
    }

    printf("  → 見つかりませんでした（比較回数: %d回）\n", comparisons);
    return -1;
}

/*
 * 二分探索
 * 探索範囲の中央値と比較し、範囲を半分に絞り込んでいく。
 * 前提条件: arrは昇順にソート済みであること。
 * 戻り値: 見つかった位置（見つからなければ -1）
 */
int binary_search(int arr[], int size, int target) {
    int low = 0;
    int high = size - 1;
    int comparisons = 0;

    while (low <= high) {
        int mid = (low + high) / 2;
        comparisons++;

        printf("  比較%d回目: low=%d, high=%d, mid=%d (arr[mid]=%d) と %d を比較\n",
               comparisons, low, high, mid, arr[mid], target);

        if (arr[mid] == target) {
            printf("  → 見つかりました（比較回数: %d回）\n", comparisons);
            return mid;
        } else if (arr[mid] < target) {
            low = mid + 1; /* 右半分に絞り込む */
        } else {
            high = mid - 1; /* 左半分に絞り込む */
        }
    }

    printf("  → 見つかりませんでした（比較回数: %d回）\n", comparisons);
    return -1;
}

int main(void) {
    int data[SIZE] = {1, 3, 5, 7, 9, 11, 13, 15, 17, 19};
    int target = 15;

    printf("探索対象の配列: ");
    print_array(data, SIZE);
    printf("探す値: %d\n\n", target);

    printf("[線形探索]\n");
    linear_search(data, SIZE, target);

    printf("\n[二分探索]\n");
    binary_search(data, SIZE, target);

    return 0;
}
