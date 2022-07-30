

# Symmetry

## Notation

`abc-symmetry message`: (Each box is a bytes, box with the same bytes is the same.)
![](https://peter.website/assets/read_symmetry.png)

`symmetry hash`: meow hash whose high 64 bit hash equal to its low 64 bit hash   


## Principle
We can find key that makes any `abc-symmetric` message, hash to an `symmetry hash` hash_val.

```python
    meow_hash(key, message) = hash_val
```

### Brief
- `AES-DEC` have no influence on symmetry
    ![](https://peter.website/assets/aes.png)
- Also... `addq`, `subq`, `xor` 
- Make message in a lane symmetric <=> make message  `abc-symmetric`
- **Zero-Pad and Len-Pad**
  -  According to `invertibility`, from fix symmetry back to key
  -  Then the key can be used to any hash any data. 

### Ex
Actually message with length != 256n bytes can also be found some universal keys that make hash `symmetric`.    
[Talk is cheap...](./meow_symmetry.py)

## Implementation


- [./meow_sym_test.py](./meow_sym_test.py)
    Test case with colored print.
    -  64-bit attack
    - '!=256n-bit' attack 
- [./meow_symmetry.py](./meow_symmetry.py) 
    Implement API:
    - `meow_symmetry` -- Get a key from a symmetry middle state 
```python 
"""  Generate a key for arbitrary hash (16 bytes) and arbitrary message (arbitrary bytes)
    :param length:         Length of origin message
    :param left_bytes:     complete 256 bytes blocks part of origin message
    :param off_bytes:      Last 32 block (len < 32) of origin message
    :param rand            Use broadcast or randomize to make a middle state
    :return:               A key that makes meow_hash(key, message) symmetric
                           And construct only one key from many possible ones
```
    - `gen_abc_symmetry_block32` -- Create a `abc-sym` message given num of blocks
    - `gen_range` -- Advanced and easy API use to implement '!=256n-bit' attack 


## Test

### Simple case (Project requirement)
- Get a key that hash 64-bit `abc-sym` message to `sym` hash
![](https://s2.loli.net/2022/07/30/a1FE4rXgUKYG3JC.png)

### Advanced case
- Get a key that hash 233-bit message to `sym` hash (blue part in message is `abc-sym` and changeable)
![](https://s2.loli.net/2022/07/30/wevTZQJOoEpcNzu.png)
- Get a key that hash 567-bit message to `sym` hash (blue part in message is `abc-sym` and changeable)
![](https://s2.loli.net/2022/07/30/c3R4OrHlLGNKBYu.png)


# Reference
- [mewo hash](https://github.com/cmuratori/meow_hash) 
- [Cryptanalysis of Meow Hash](https://peter.website/meow-hash-cryptanalysis)
