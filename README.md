
# Introduction
网络空间安全创新创业实践
(Cyberspace Security Innovation and entrepreneurship practice project)

202000460071 Xiaohan Ni

## Status
| Project                                                      | Readme                                                       |
| ------------------------------------------------------------ | ------------------------------------------------------------ |
| Do your best to optimize SM3 implementation (software)       | [SM3](sm3/README.md)                                         |
| Implement the naïve birthday attack of reduced SM3           | [Hash Collision](collision/README.md) ,  [Birthday Attack](collision/birthday_attack/README.md) |
| Implement the Rho method of reduced SM3                      | [Hash Collision](collision/README.md) , [Rho Attack](collision/rho_attack/README.md) |
| Implement length extension attack for SM3, SHA256            | [Length Extend Attack](./lenExAttack/README.md)              |
| Implement Merkle Tree following RFC6962                      | [Merkle Tree](./merkleTree/README.md)                        |
| Find a key with hash value `sdu_cst_20220610` under a message composed of your name followed by your student ID. For example, `San Zhan 202000460001`. | [mewo](./meow/README), [invertibility](./meow/mewo_inv/README.md)                   |
| Find a 64-byte message under some $k$ fulfilling that their hash value is symmetrical | [mewo](./meow/README.md), [symmetry](./meow/mewo_sym/README.md)      |
| Other...                                                     | [SM4](sm4/README.md) , [SM2](sm2/README.md)                  |






# Build
## Python
### virtual env
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

## C/C++
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
