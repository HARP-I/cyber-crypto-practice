# Introduction

SM3 is a cryptographic hash algorithm published by OSCCA as an authorized cryptographic hash algorithm for the use within China. It is intended to address multiple use cases for commercial cryptography, including, but not limited to: Digital signatures, MAC, RNG.

# Overview of performance

**Hash  about 1GiB random data**

| Version    | Time measure |
| ---------- | ------------ |
| sm3_origin | 20s+         |
| sm3_opt    | 4.56s        |
| openssl    | 3.97s        |
| gmssl      | 11.04s       |





# Implementation

- `./sm3_base`: Trival Implementation
  - LOOK-UP TABLE
- `./sm3_unroll`: Optimized by loop unroll implementation
  - UNROOL
  - LOOK-UP TABLE
- `./sm3_simd`: Optimized by simd implementation
  - LOOK-UP TABLE
  - UNROOL
  - SIMD
- `./sm3_reduced`: round-reduced sm3 (add round options in api)

# Build
`cd` to root directory and use your build tools.
For my environment(win10, mingw):

```bash
mkdir build
cd build
cmake .. -G="MingW Makefiles"
make
```
Then there will be some subdirectories
- `.\bin`: Executable test programs of sm3 
- `.\lib`: Static lib of different versions of sm3

If your machine don't support `ssse3`, you probably need to uncomment SIMD entry in `CMakeLists.txt`

# Test
> Use openssl/gmssl as benchmark (such as `echo -n "abc" | gmssl sm3`)
The following result(snip with white backgound) is from `sm3_test_simd`. 
( `sm3_unroll` and `sm3_test_base`  is equally valid and checkable)

- Digest of "abc"
![](https://s2.loli.net/2022/07/27/aHJ35AVSyMjkQrX.png)
![](https://s2.loli.net/2022/07/27/QjJMFIyo9rALfpm.png)

- Digest of "abcdabcdabcdabcdabcdabcdabcdabcdabcdabcdabcdabcdabcdabcdabcdabcd"
![](https://s2.loli.net/2022/07/27/1wQiWSGRPkFhDlE.png)
![](https://s2.loli.net/2022/07/27/CQ6TUDHzhX7inkV.png)

- Digest of a large file (> 1G)
To get a benchmark, create a large random file by `gmssl rand -outlen 1111111111 newfile.txt`
Then:  
![](https://s2.loli.net/2022/07/27/Ceid4My1vzZIhj7.png)

The test file is to big to upload so the primary test file is also not provided intallation in CMake, but you can hash any file to test because `sm3_test_simd` can regard a arg as filename to hash like: 
![](https://s2.loli.net/2022/07/27/SDyElwCH2AUQ5Gv.png)
> To show how many content has been hash, you can manully open the flag by uncomment the `#define Progress_Log` in[source code](./test/src/main.c) and rebuild.
![](https://s2.loli.net/2022/07/27/xtD8Ff2nXpWcerk.png)

- Test of round-reduced sm3: `sm3_test_reduced` can take first arg as rounds.
  ![](https://s2.loli.net/2022/07/27/KS423U1GJrWh7VH.png)
  ![](https://s2.loli.net/2022/07/27/xMOwPXYugGrEdnm.png)

# Principle

## Overview
MD Structure
- Padding
    - Similar to SHA256
    - Brief: 1 + (some 0, >= 1) + (len of msg) 
    - Len of msg： < 2^64
    - Len of block： 512
    - Len of digest： 256

- Iterative compression
    - Num of Rounds = Num of Block
    - <img src="https://s2.loli.net/2022/07/19/mdtULePWYZ1loaV.png"/>
    - V0 -- initialized vector

## Compress 
Message Expansion + Compress 

- Message Expansion
    - Split Again
    - Iterative Expansion
    <img src="https://s2.loli.net/2022/07/19/fntPQrhKmx6zCOi.png"/> 

- Compress Func
<img src="https://s2.loli.net/2022/07/19/hnyDX4jm6PsL7qS.png" width="75%">

## Optimization

- Look-up table
  Precomputation: replace rotate shift left operation with table look-up operation. 
  ```(Txx <<< j) -> K[j]```
  ![](https://s2.loli.net/2022/07/31/xGfZTPIeQg9hwtz.png)
  - Similarly, most function is replaced with macro to eliminate stack operation.
- Loop unrolling (xmacro from gmssl)
  - `for` was unrolled by calling macro. 
  - Reduce branching and operation not related to calculate hash.
  - Make a little modification on one round, only update but not shift the whole block.
  - ![](https://s2.loli.net/2022/07/31/zkTUEm3bO8vHjpq.png)
  - Then shift a block each round; eight round = one cycle.
- SIMD
  - SIMD is used to parallelly generate W.
  - For W0 ~ W15:
  ```c
            X = _mm_shuffle_epi8 (
              _mm_loadu_si128 ((__m128i *)(input + (j << 2))), //
              _mm_setr_epi8 (3, 2, 1, 0, 7, 6, 5, 4, 11, 10, 9, 8, 15, 14, 13,
                             1)  //
  ```
  Absorb four words(32 bytes blocks) each time. For each word, to read bytes in `big endian` and store it in a `uint32_t`. Use `shuffle` to reversed order of each four bytes.
  - For W16 ~ W67:
    - It's **impossible** to calcualte a vector of `X = (W[j - 3], W[j - 2], W[j - 1], W[j]) `. Because the `W[j]` is up to W[j - 3] -- they are not independent in dataflow.
    - But we can divide the vector into two part:
      - `X = Xa + Xb = (W[j - 3], W[j - 2], W[j - 1], 0) '+' (0 , 0, 0, W[j])`
      - Each time we jump four step, but each loop become consumed double time than before. As a result, the time measure in this part should be half.
    - For detailed description, look through the code comment. Actually simd trick like 'broadcast' and math trick like 'differential sequence' are combined. 

<!-- ![](https://media.giphy.com/media/oZ911NGZED3iQqmYcn/giphy.gif) -->

# Reference
- [wiki](https://en.wikipedia.org/wiki/SM3_(hash_function))
- [ietf-doc](https://tools.ietf.org/id/draft-oscca-cfrg-sm3-02.html)
- [zh-doc](https://www.oscca.gov.cn/sca/xxgk/2010-12/17/1002389/files/302a3ada057c4a73830536d03e683110.pdf)
- [gmssl](https://github.com/guanzhi/GmSSL/blob/develop/src/sm3.c)
- [index.html](https://www.intel.com/content/www/us/en/docs/intrinsics-guide/index.html)
- `./ref` list Referred Implementation
    - `./ref/sm3_std`: From ietf doc
    - `./ref/sm3_gmssl`: From gmssl 


## An Interesting Idea 
To makes the program hash itself

```
./sm3_test_simd ./sm3_test_simd```
