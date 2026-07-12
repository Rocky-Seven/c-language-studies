package main

import "fmt"

func printArray(arr []int) {
	for _, v := range arr {
		fmt.Printf("%d ", v)
	}
	fmt.Println()
}

func bubbleSort(arr []int) {
	n := len(arr)

	for i := 0; i < n-1; i++ {
		swapped := false

		for j := 0; j < n-1-i; j++ {
			if arr[j] > arr[j+1] {
				arr[j], arr[j+1] = arr[j+1], arr[j] // 多重代入で1行で交換できる
				swapped = true
			}
		}

		fmt.Printf("%d回目のパス後: ", i+1)
		printArray(arr)

		if !swapped {
			break
		}
	}
}

func main() {
	data := []int{5, 2, 8, 1, 9, 3}

	fmt.Print("ソート前: ")
	printArray(data)

	bubbleSort(data)

	fmt.Print("ソート後: ")
	printArray(data)
}
