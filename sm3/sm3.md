# Introduction

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

## Application
- Digital signatures
- MAC
- RNG

# Implementation

- `/sm3/sm3_base`: Trival Implementation
  - LOOK-UP TABLE
- `/sm3/sm3_unroll`: Optimized implementation
  - UNROOL
  - LOOK-UP TABLE
- `/sm3/sm3_unroll`: Optimized implementation
  - UNROOL
  - LOOK-UP TABLE

- `/sm3/ref` Ref Implementation
    - `/sm3/ref/sm3_std`: From ietf doc
    - `/sm3/ref/sm3_gmssl`: From gmssl 

# Usage

## Requirement: 
- cmake
- gcc

## Run Testcase
```bash
    mkdir build
    cd build
    cmake ..
    ./main
```


# Reference
- [wiki](https://en.wikipedia.org/wiki/SM3_(hash_function))
- [ietf-doc](https://tools.ietf.org/id/draft-oscca-cfrg-sm3-02.html)
- [zh-doc](https://www.oscca.gov.cn/sca/xxgk/2010-12/17/1002389/files/302a3ada057c4a73830536d03e683110.pdf)
- [gmssl](https://github.com/guanzhi/GmSSL/blob/develop/src/sm3.c)
- [index.html](https://www.intel.com/content/www/us/en/docs/intrinsics-guide/index.html)