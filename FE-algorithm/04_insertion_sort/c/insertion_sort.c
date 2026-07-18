#include <stdio.h>

#define SIZE 6

void print_array(int arr[], int size) {
    for (int i = 0; i < size; i++) {
        printf("%d ", arr[i]);
    }
    printf("\n");
}

void insertion_sort(int arr[], int size) {
    for (int i = 1; i < size; i++) {
        int key = arr[i];
        int j = i - 1;

        while (j >= 0 && arr[j] > key) {
            arr[j + 1] = arr[j];
            j--;
        }
        arr[j + 1] = key;

        printf("%d回目のパス後: ", i);
        print_array(arr, size);
    }
}

int main(void) {
    int data[SIZE] = {5, 2, 8, 1, 9, 3};

    printf("ソート前: ");
    print_array(data, SIZE);

    insertion_sort(data, SIZE);

    printf("ソート後: ");
    print_array(data, SIZE);

    return 0;
}
