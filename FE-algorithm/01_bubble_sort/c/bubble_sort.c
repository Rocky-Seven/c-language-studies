#include <stdio.h>

#define SIZE 6

/* 配列の中身を表示する関数 */
void print_array(int arr[], int size) {
    for (int i = 0; i < size; i++) {
        printf("%d ", arr[i]);
    }
    printf("\n");
}

/* バブルソート本体 */
void bubble_sort(int arr[], int size) {
    for (int i = 0; i < size - 1; i++) {
        int swapped = 0; /* 入れ替えが発生したかどうかのフラグ */

        for (int j = 0; j < size - 1 - i; j++) {
            if (arr[j] > arr[j + 1]) {
                /* 隣同士を入れ替える */
                int temp = arr[j];
                arr[j] = arr[j + 1];
                arr[j + 1] = temp;
                swapped = 1;
            }
        }

        printf("%d回目のパス後: ", i + 1);
        print_array(arr, size);

        /* 1回も入れ替えが起きなければ、その時点でソート完了 */
        if (swapped == 0) {
            break;
        }
    }
}

int main(void) {
    int data[SIZE] = {5, 2, 8, 1, 9, 3};

    printf("ソート前: ");
    print_array(data, SIZE);

    bubble_sort(data, SIZE);

    printf("ソート後: ");
    print_array(data, SIZE);

    return 0;
}
