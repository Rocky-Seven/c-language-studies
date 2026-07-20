fun selectionSort(arr: IntArray) {
    for (i in 0 until arr.size - 1) {
        var minIdx = i
        for (j in i + 1 until arr.size) {
            if (arr[j] < arr[minIdx]) {
                minIdx = j
            }
        }
        if (minIdx != i) {
            val temp = arr[i]
            arr[i] = arr[minIdx]
            arr[minIdx] = temp
        }
        println("${i + 1}回目のパス後: ${arr.joinToString(" ")}")
    }
}

fun main() {
    val data = intArrayOf(5, 2, 8, 1, 9, 3)

    println("ソート前: ${data.joinToString(" ")}")

    selectionSort(data)

    println("ソート後: ${data.joinToString(" ")}")
}
