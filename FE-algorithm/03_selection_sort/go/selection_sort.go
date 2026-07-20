package main

import "fmt"

func selectionSort(arr []int) {
	n := len(arr)
	for i := 0; i < n-1; i++ {
		minIdx := i
		for j := i + 1; j < n; j++ {
			if arr[j] < arr[minIdx] {
				minIdx = j
			}
		}
		if minIdx != i {
			arr[i], arr[minIdx] = arr[minIdx], arr[i]
		}
		fmt.Printf("%d回目のパス後: %v\n", i+1, arr)
	}
}

func main() {
	data := []int{5, 2, 8, 1, 9, 3}

	fmt.Println("ソート前:", data)

	selectionSort(data)

	fmt.Println("ソート後:", data)
}
