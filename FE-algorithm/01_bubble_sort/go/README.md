# Go版

C言語版のバブルソートをGoに移植したものです。

## 実行方法（Codespaces等、Go導入済み環境で）

```bash
go run bubble_sort.go
```

## C言語版との違い

- **交換処理**：Cでは `temp` を使った3行の入れ替えが必要でしたが、Goは `arr[j], arr[j+1] = arr[j+1], arr[j]` という多重代入で1行にまとめられます
- **配列の長さ**：Cでは `#define SIZE` で自分で管理していましたが、Goは `len(arr)` でいつでも取得できます
- **表示処理**：`fmt.Printf` はCの `printf` とほぼ同じ感覚で書けるので、Cを知っていれば読みやすいはずです


