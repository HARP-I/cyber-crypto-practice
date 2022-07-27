
# Introduction

Collision attacks against SM3. [(About Hash Collison)](https://en.wikipedia.org/wiki/Hash_collision)
To get more detailed results, standard sm3 and various types of **Round-Reduced SM3** are applied. 

Tow kinds policies Included:
- [Naïve birthday attack](./birthday_attack/README.md)
  - Classical method
  - Three new methods proposed
- [The Rho method](./rho_attack/README.md)


Attack Code:
- `.py` Third-party library based implementation
- `.c` No dependency based implementation

# Result
- Search by birthday attack (logInfo: each line is a pair of digest and its preimage)
  ![Search by birthday attack](https://s2.loli.net/2022/07/27/gtL6MBvI38bYwhA.png)
- Result of birthday attack

- Result of Rho attack



# Build

To build the project, you are recommanded to setup a virtual env.

First type the following command in the root directory:
```bash
python -m venv venv
```
Then activate the venv
```bash
./venv/Script/activate
```
To exit, just type
```
deactivate
```


# Reference
- [Hash Collison](https://en.wikipedia.org/wiki/Hash_collision)
