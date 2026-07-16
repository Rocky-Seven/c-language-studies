#include <stdio.h>

#define SIZE 6

void print_array(int arr[], int size) {
    for (int i = 0; i < size; i++) {
        printf("%d ", arr[i]);
    }
    printf("\n");
}

void selection_sort(int arr[], int size) {
    for (int i = 0; i < size - 1; i++) {
        int min_idx = i;

        for (int j = i + 1; j < size; j++) {
            if (arr[j] < arr[min_idx]) {
                min_idx = j;
            }
        }

        if (min_idx != i) {
            int temp = arr[i];
            arr[i] = arr[min_idx];
            arr[min_idx] = temp;
        }

        printf("%d回目のパス後: ", i + 1);
        print_array(arr, size);
    }
}

int main(void) {
    int data[SIZE] = {5, 2, 8, 1, 9, 3};

    printf("ソート前: ");
    print_array(data, SIZE);

    selection_sort(data, SIZE);

    printf("ソート後: ");
    print_array(data, SIZE);

    return 0;
}
