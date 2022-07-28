# Introduction

SM3 is a cryptographic hash algorithm published by OSCCA as an authorized cryptographic hash algorithm for the use within China. It is intended to address multiple use cases for commercial cryptography, including, but not limited to: Digital signatures, MAC, RNG.

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

## Time

# Build
Change to root directory and use your build tools.
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

## Optimized

- Look-up table
Replace rotate shift left operation with table look up  

SIMD can optimize sm3



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
Actully if permitted, you can do:
```bash
./sm3_test_simd ./sm3_test_simd
```
It makes the program hash itself...

