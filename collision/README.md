
# Introduction

Collision attacks against SM3. [(About Hash Collison)](https://en.wikipedia.org/wiki/Hash_collision)
To get more detailed results, standard sm3 and various types of **Round-Reduced SM3** are applied. 

Tow kinds policies Included(for detailed result):
- [Naïve birthday attack](./birthday_attack/README.md)
  - Classical method
  - Three new methods proposed
- [The Rho method](./rho_attack/README.md)


<!-- Attack Code:
- `.py` Third-party library based implementation
- `.c` No dependency based implementation -->


# Build

Python's virtual env is recommanded.

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
