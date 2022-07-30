
# Invertibility

## Principle
Because each step is invertibale in hash, if `meow_hash` denote a keyed hash:
```
    meow_hash(key, message) = hash_val
```
Then we can calculate the result `meow_invert` meeting:
```
    meow_hash(meow_invert(hash_val, message), message) == hash_val
```

But in this case, maybe `key1 != key2`:
```
    meow_invert(meow_hash(key1, message), message) == key2 
```
In another word, `meow_invert` can be a non-deterministic algorithm. Because `Aggregation` is a compress, mapping 1024 bits to 128 bits. An hash have mutliple preimage. An preimage of fixed hash can be set by iterating the dangling 7 lanes. 

To get the inverse of finalization and absorbtion(`get-inverse`):
- Reverse the order of calling each subprocess
- For each subprocess (Like one round absorb or one round finalize):
  - For node process: Recursively `get-inverse`
    - `aes_dec` to `aes_enc` ??
      - `AddRoundKey` to `AddRoundKey`
      - `R-MixCol` to `MixCol` 
      - `R-Sbox` to `Sbox`
      - `R-ShiftRow` to `ShiftRow`
  - For leaf process: (mainly rely on math, for example)
    - `paddq` to `psubq`
    - `pxor` to `pxor`



## Implementation
- [`./meow/include`](../include/meow.py)
  meow hash implementation from [Peter](https://peter.website/assets/full_mh.pngs)
- [`./meow_inv_test.py`](./meow_inv_test.py)
    test case
- `./meow_inverse.py`(./meow_inverse_test.py)
  some inverse function implementation
- [`../include/util.py`](../include/util.py)
  const (sbox, inverse sbox) and function used in meow implementation.

Detailed description can be found in the code comment.

## Test
- Input: 
  - Message: Name + School ID
  - Hash_val: sdu_cst_20220610
![](https://s2.loli.net/2022/07/30/XHk9dA2caVy4eY1.png)

Default deterministic, to make output key non-deterministic, set ``rand=Ture`` 
```
  re_key = meow_inverse(hash_val, message, rand=True)
```

# Reference
- [mewo hash](https://github.com/cmuratori/meow_hash) \
- [Cryptanalysis of Meow Hash](https://peter.website/meow-hash-cryptanalysis)
