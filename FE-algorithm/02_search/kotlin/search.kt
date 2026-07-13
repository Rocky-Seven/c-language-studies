fun printArray(arr: IntArray) {
    println(arr.joinToString(" "))
}

fun linearSearch(arr: IntArray, target: Int): Int {
    var comparisons = 0

    for (i in arr.indices) {
        comparisons++
        println("  比較${comparisons}回目: arr[$i] = ${arr[i]} と $target を比較")

        if (arr[i] == target) {
            println("  → 見つかりました（比較回数: ${comparisons}回）")
            return i
        }
    }

    println("  → 見つかりませんでした（比較回数: ${comparisons}回）")
    return -1
}

fun binarySearch(arr: IntArray, target: Int): Int {
    var low = 0
    var high = arr.size - 1
    var comparisons = 0

    while (low <= high) {
        val mid = (low + high) / 2 // 整数除算なので端数は切り捨てられる
        comparisons++

        println("  比較${comparisons}回目: low=$low, high=$high, mid=$mid (arr[mid]=${arr[mid]}) と $target を比較")

        when {
            arr[mid] == target -> {
                println("  → 見つかりました（比較回数: ${comparisons}回）")
                return mid
            }
            arr[mid] < target -> low = mid + 1  // 右半分に絞り込む
            else -> high = mid - 1              // 左半分に絞り込む
        }
    }

    println("  → 見つかりませんでした（比較回数: ${comparisons}回）")
    return -1
}

fun main() {
    val data = intArrayOf(1, 3, 5, 7, 9, 11, 13, 15, 17, 19)
    val target = 15

    print("探索対象の配列: ")
    printArray(data)
    println("探す値: $target\n")

    println("[線形探索]")
    linearSearch(data, target)

    println("\n[二分探索]")
    binarySearch(data, target)
}
