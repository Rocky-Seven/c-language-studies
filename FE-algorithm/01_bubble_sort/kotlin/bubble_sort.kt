fun printArray(arr: IntArray) {
    println(arr.joinToString(" "))
}

fun bubbleSort(arr: IntArray) {
    val n = arr.size

    for (i in 0 until n - 1) {
        var swapped = false

        for (j in 0 until n - 1 - i) {
            if (arr[j] > arr[j + 1]) {
                // also を使うと一時変数を明示的に書かずに交換できる
                arr[j] = arr[j + 1].also { arr[j + 1] = arr[j] }
                swapped = true
            }
        }

        print("${i + 1}回目のパス後: ")
        printArray(arr)

        if (!swapped) {
            break
        }
    }
}

fun main() {
    val data = intArrayOf(5, 2, 8, 1, 9, 3)

    print("ソート前: ")
    printArray(data)

    bubbleSort(data)

    print("ソート後: ")
    printArray(data)
}
