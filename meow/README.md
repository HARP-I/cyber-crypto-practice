
# Introduction
Implement some 'attacks' on the hash algorithm [meow](https://github.com/cmuratori/meow_hash), based on properties of 'Invertibility' and 'Symmetry'.
> Attacks is placed inside '', because the following attacks are both not direct threat to meow hash. However, the meow hash has been break to an non-cryptographic checksum due to [Peter](https://peter.website/meow-hash-cryptanalysis).

 

# Principle
An overview of how meow work.
A sponge-like hash: padding + absorption +  finalizatoin + aggregation

- Absorption:
    - Sequence diagram
    ![](https://peter.website/assets/meow_hash_shift_register.png)
    - Each time absorb 32 bytes (`M` in the dragram)
    - Block in  `i` move to `i + 1`
    <!-- - Lane diagram
    ![](https://peter.website/assets/mh_absorption_function.png)  -->
- Padding and absorption order:
    - Each time absorb complete 256 bytes in order 
    - If not enough, zero-pad to next 32 bytes in tail
    - Absorb zero-paded block
    - Absorb length blobk
    - Absorb the last few blocks in order (if exist)
- Finalization
    - Sequence diagram
    [](https://peter.website/assets/meow_hash_finalization_shift_register.png)
    - No message need to be absorbed
    - 12 rounds
    <!-- - Lane diagram
    ![](https://peter.website/assets/mh_finalization.png)  -->
- Aggregation
    - One round
    - Compress 8 lanes to 1 lane as output


For detail about each 'attack':
- [Invertibility](./mewo_inv/README.md)
- [Symmetry](./mewo_sym/README.md)
To check the result of 'attack', turn to  and 

## Invertibility
![](https://s2.loli.net/2022/07/30/3D49ukXv8aHyTVz.png)


## Symmetry
![](https://s2.loli.net/2022/07/30/a1FE4rXgUKYG3JC.png)
![](https://s2.loli.net/2022/07/30/wevTZQJOoEpcNzu.png)
![](https://s2.loli.net/2022/07/30/c3R4OrHlLGNKBYu.png)
![](https://s2.loli.net/2022/07/30/2mJw9TUz7sbt3Ll.png)
![](https://peter.website/assets/read_symmetry.png)


# Reference
- [mewo hash](https://github.com/cmuratori/meow_hash) \
- [Cryptanalysis of Meow Hash](https://peter.website/meow-hash-cryptanalysis)
