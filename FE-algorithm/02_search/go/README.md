# Go版

C言語版の線形探索・二分探索をGoに移植したものです。

## 実行方法（Codespaces等、Go導入済み環境で）

```bash
go run search.go
```

## C言語版との違い

- **配列の走査**：Cでは添字 `i` を自分で管理して `arr[i]` を取り出しますが、Goは `for i, v := range arr` で添字と値を同時に取得できます
- **配列の長さ**：Cでは `#define SIZE` で自分で管理していましたが、Goは `len(arr)` でいつでも取得できます
- **表示処理**：`fmt.Printf` はCの `printf` とほぼ同じ感覚で書けるので、Cを知っていれば読みやすいはずです
