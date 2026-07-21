package main

import "fmt"

func insertionSort(arr []int) {
	for i := 1; i < len(arr); i++ {
		key := arr[i]
		j := i - 1
		for j >= 0 && arr[j] > key {
			arr[j+1] = arr[j]
			j--
		}
		arr[j+1] = key
		fmt.Printf("%d回目のパス後: %v\n", i, arr)
	}
}

func main() {
	data := []int{5, 2, 8, 1, 9, 3}

	fmt.Println("ソート前:", data)

	insertionSort(data)

	fmt.Println("ソート後:", data)
}
