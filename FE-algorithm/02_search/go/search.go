package main

import "fmt"

func printArray(arr []int) {
	for _, v := range arr {
		fmt.Printf("%d ", v)
	}
	fmt.Println()
}

func linearSearch(arr []int, target int) int {
	comparisons := 0

	for i, v := range arr {
		comparisons++
		fmt.Printf("  比較%d回目: arr[%d] = %d と %d を比較\n", comparisons, i, v, target)

		if v == target {
			fmt.Printf("  → 見つかりました（比較回数: %d回）\n", comparisons)
			return i
		}
	}

	fmt.Printf("  → 見つかりませんでした（比較回数: %d回）\n", comparisons)
	return -1
}

func binarySearch(arr []int, target int) int {
	low := 0
	high := len(arr) - 1
	comparisons := 0

	for low <= high {
		mid := (low + high) / 2 // 整数除算なので端数は切り捨てられる
		comparisons++

		fmt.Printf("  比較%d回目: low=%d, high=%d, mid=%d (arr[mid]=%d) と %d を比較\n",
			comparisons, low, high, mid, arr[mid], target)

		if arr[mid] == target {
			fmt.Printf("  → 見つかりました（比較回数: %d回）\n", comparisons)
			return mid
		} else if arr[mid] < target {
			low = mid + 1 // 右半分に絞り込む
		} else {
			high = mid - 1 // 左半分に絞り込む
		}
	}

	fmt.Printf("  → 見つかりませんでした（比較回数: %d回）\n", comparisons)
	return -1
}

func main() {
	data := []int{1, 3, 5, 7, 9, 11, 13, 15, 17, 19}
	target := 15

	fmt.Print("探索対象の配列: ")
	printArray(data)
	fmt.Printf("探す値: %d\n\n", target)

	fmt.Println("[線形探索]")
	linearSearch(data, target)

	fmt.Println("\n[二分探索]")
	binarySearch(data, target)
}
