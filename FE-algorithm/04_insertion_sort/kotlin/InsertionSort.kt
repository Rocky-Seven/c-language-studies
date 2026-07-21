fun insertionSort(arr: IntArray) {
    for (i in 1 until arr.size) {
        val key = arr[i]
        var j = i - 1
        while (j >= 0 && arr[j] > key) {
            arr[j + 1] = arr[j]
            j--
        }
        arr[j + 1] = key
        println("${i}回目のパス後: ${arr.joinToString(" ")}")
    }
}

fun main() {
    val data = intArrayOf(5, 2, 8, 1, 9, 3)

    println("ソート前: ${data.joinToString(" ")}")

    insertionSort(data)

    println("ソート後: ${data.joinToString(" ")}")
}
