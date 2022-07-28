
# Introduction
Length-extension attack can be a threat to hash function with **Merkle–Damgård(MD)** construction

![](https://s2.loli.net/2022/07/27/9L86IbTeoZlR4Gk.png)


## Brief
Basically, if you know $Hash(M)$ for some unknown $M$, composed of blocks $M_1$ and $M_2$ (after padding), you can determine $Hash(M_1 || M_2 || M_3)$ for any block, $M3$. 

Because the hash of $M1 || M2$ is the chaining value that follows immediately after $M_2$, you can add another block, $M_3$, to the hashed message, even though you don’t know the data that was hashed. And it can be generalized to any number of blocks, either in the unknown message part or the suffix part


## Generalization

It can be generalized to the following cases, in the perspective of attacker. 

Notations:
- $pad(X) = X || padX $
- $Hash(X) = Hash(IV, pad(X))$
### Case1
- Unknown: $M$ (or partly unknown)
- Known: $Hash(M), len(M)$
  
Forgery: $Hash(M || padM || N) = Hash(Hash(M), N || padN)$
Possible by: $len(M) \rightarrow padM \rightarrow len(M || padM || N) \rightarrow padN$


### Case2

- Unknown: $M$ (or partly unknown)
- Known: $Hash(M)$
  
Forgery: $Hash(pad(M) || N) = Hash(Hash(M), N || padN)$
> Though $len(M)$ is unknown, we can guess it, try different  padN. 


### Case3
> Notation: let $M = M_1 || M_2$, know $M_2$, not know $M_1$

- Unknown: $M_1$
- Known: $Hash(M), M_2, len(M)$ 

Forgery:
- $P_1 = Hash(M || padM || N) = Hash(Hash(M), N || padN)$
- $P_2 = M_2 || padM || N$

Possible by:
- $len(M_1 || M_2) \rightarrow padM \rightarrow len(M || padM || N) \rightarrow padN$
- $len(M_1 || M_2) \rightarrow padM$


> Two target don't make it much harder. Similarly, we need only guess $len(M)$ when $len(M)$ is unknown. Call it `case3*`.


### Case4
- Unknown: $M_1, len(M)$
- Known: $Hash(M_1 || M_2), M_2$

Forgery:
- $P_1, P_2$ as long as $Hash(M_1 || P_2) == P_1$
<!-- - $P_1 = Hash(M || padM || N) = Hash(Hash(M), N || padN)$
- $P_2 = M_2 || padM || N$ -->
  
> It can be found that successful forgery in `case 3*` can also pass here.

In another word, we can make forgery in case 3 without $len(M)$:
- Guess len(M) 
- Generate guessed padN, padM
- Generate guessed forged pair

And in `case 3*`, forgery is vaild if and only if:
- both $P_1, P_2$ is valid <-> len(M) is right guess
But here, forgery is vaild if:
- $len(M)$ is right -- forgery(pair) must be valid
- $len(M)$ is wrong -- forgery(pair) might still be valid(how much possibility?)


## Implementation
We attack the classic `case4`.
Forgery: $P_1, P_2$ as long as $Hash(M_1 || P_2) == P_1$

- Attack with `len`
Need only once hash. We briefly change the `sm3` hash function so that it can hash from a middle state. 
![](https://s2.loli.net/2022/07/28/ZwzKbkl2AXSxcBo.png)

- Attack without `len`
We generate the secret as long as possible and it is still easily forgery by a iteration of guess.
![](https://s2.loli.net/2022/07/28/yNWnClerOwGcbB1.png)
![](https://s2.loli.net/2022/07/28/IjVZ4cenPWykq5O.png)



## To do
- Detailed proof (possibility?) in case4?

# Reference
- [Serious Cryptography](https://zh.book4you.org/book/3399523/14ceb5)
- [Everything you need to know about hash length extension attacks](https://blog.skullsecurity.org/2012/everything-you-need-to-know-about-hash-length-extension-attacks)