
# Introduction
网络空间安全创新创业实践课设
(Cyberspace Security Innovation and entrepreneurship practice project)

# Status (完成情况)
TODO
Project: implement the naïve birthday attack of reduced SM3
Project: implement the Rho method of reduced SM3
Project: implement length extension attack for SM3, SHA256, etc.
Project: do your best to optimize SM3 implementation (software)

Project: Impl Merkle Tree following RFC6962

✅Project: Try to Implement this scheme

SM2：
✅Project: report on the application of this deduce technique in Ethereum with ECDSA

Project: impl sm2 with RFC6979

Project: verify the above pitfalls with proof-of-concept code

Project: Implement the above ECMH scheme

Project: Implement a PGP scheme with SM2

Project: implement sm2 2P sign with real network communication

Project: PoC impl of the scheme, or do implement analysis by Google

Project: implement sm2 2P decrypt with real network communication

Bitcoin-public：
Project: send a tx on Bitcoin testnet, and parse the tx data down to every bit, better write script yourself

Project: forge a signature to pretend that you are Satoshi

Ethernet-public：
Project: research report on MPT



Real-world-cryptanalysis：
✅Project: Find a key with hash value “*sdu_cst_20220610*” under a message composed of *your name* followed by *your student ID*. For example, “*San Zhan 202000460001*”.

Project: Find a 64-byte message under some k fulfilling that their hash value is symmetrical

Real-world-ZK：
Project: Write a circuit to prove that your CET6 grade is larger than 425.

a.Your grade info is like (cn_id, grade, year, sig_by_moe). These grades are published as commitments onchain by MoE.

b.When you got an interview from an employer, you can prove to them that you have passed the exam without letting them know the exact grade.

Project: The commitment scheme used by MoE is SHA256-based.

a. commit = SHA256(cn_id, grade, year, sig_by_moe, r)****


- [SM3](sm3/README.md)
- [SM4](sm4/README.md)
- [Hash Collision](collision/README.md)
  - [Birthday Attack](collision/birthday_attack/README.md)
  - [Rho Attack](collision/rho_attack/README.md)


# Build

Generally use CMake(3.5+) to build c/c++ project.
Choose your build tools use `-G`.
```bash
mkdir build
cd build
cmake .. -G="MingW Makefiles"
make
```
Or:
```
cmake -S . -B build
cmake --build build
```

- `build\bin`: Executable test programs 
- `build\lib`: Static lib


`*.py` can directly run in interpreter.

## virtual env
For python, virtual env is recommanded. 
Type the following command in terminal. 
```bash
python -m venv venv
```
Activate the venv
```bash
./venv/Script/activate
```
To exit:
```bash
deactivate
```

## Python Library
```bash
# provide api for sm3
pip install snowland-smx==0.3.1
```
