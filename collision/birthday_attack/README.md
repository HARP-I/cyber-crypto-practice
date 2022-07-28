
## Iteraition Attack
Reduced sm3 by cut(Truncate few bits)
> Fail to search collisions in full sm3.
> Not attempted to do search collisions in round-reduced sm3.
- Birthday Attack(online)
![](https://s2.loli.net/2022/07/27/leY2QFwWzViADbT.png)
- Bithday Attack(Iteration)
![](https://s2.loli.net/2022/07/27/62tQu7cKosENZiL.png)
```


## Principle of Birthday Attack
- [Birthday problem](https://en.wikipedia.org/wiki/Birthday_problem)
- [Pigeonhole principle](https://en.wikipedia.org/wiki/Pigeonhole_principle)

**Prerequisite**    
- $f(x)$ yields any of $H$ different outputs with equal probability
- $H \gg n$
- Evaluating $f$ for about $1.25\sqrt{H}$ different arguments on average
With above, we expect to obtain a collision. 

**Math conclusion**

- Collision probability (randomly n preimages)
$${\displaystyle p(n;H)\approx 1-e^{-n(n-1)/(2H)}\approx 1-e^{-n^{2}/(2H)}}$$

- Lowerbound 
$${\displaystyle n(p;H)\approx {\sqrt {2H\ln {\frac {1}{1-p}}}}}$$
$${\displaystyle n(0.5;H)\approx 1.1774{\sqrt {H}}}$$

- Expectations
$$Q(H) \approx {\sqrt {{\frac {\pi }{2}}H}}\quad \left(\approx \sum{n \cdot p(n;H)} \right)$$

> Note: expections of random preimage set size =? expections of attemps 


## Implementation

### Naive idea
According to [Serious Cryptography](https://zh.book4you.org/book/3399523/14ceb5)
Here’s the simplest way to carry out the birthday attack in order to find
collisions:
1. Compute $2^{n/2}$ hashes of $2^{n/2}$ arbitrarily chosen messages and store all
the message/hash pairs in a list.
2. Sort the list with respect to the hash value to move any identical hash
values next to each other.
3. Search the sorted list to find two consecutive entries with the same
hash value.


### Other idea

Storing pair in `map` seems a good idea. 

To make attack as efficient as possible, we the following building blocks.

**The main subprocess**  
- `RNG`: random number(preimage) generation  
  - cryptographically secure?
  - is random necessary ?
- `Hash` hash each generated random number


**Derived schemes**

The process of `RNG` and `Hash` can be "offline" or "online". [Online/Offline](https://en.wikipedia.org/wiki/Online_algorithm)

- Online `RNG`, Online `Hash`
  - Generate a random  number, hash it as key
  - If the map[hash] exist and not equal to the new preimage means collison,  else store the pair and repeat the operation 

- Offline `RNG`, Online `Hash`:
  - Generate the whole preimage set (large enough)
  - Each time hash a element as key
  - If the map[hash] exist and not equal to the new preimage means collison,  else store the pair and repeat the operation
  

But there seems no need to make consistant RNG or to take more space to save the set of preimage.  


More ambiguous idea: Offline `RNG`, Offline `Hash`.  Maybe it can be interpreted after importing idea of distributed computing: 
- Each machine/thread can do hash and **maintain its own map**
- If no party find collison during long time, we can **merge the map together** to find if there are same data (Intersection)


# Defects
In a words, requires a lot of memory.
See [Rho method](../rho_attack/README.md]) to learn about a low-memory attack.


# Reference
<!-- - [Bithday Attack](https://en.wikipedia.org/wiki/Birthday_attack) -->
- [Birthday Problem](https://en.wikipedia.org/wiki/Birthday_problem)
- [Pigeonhole Principle](https://en.wikipedia.org/wiki/Pigeonhole_principle)
- [Serious Cryptography](https://zh.book4you.org/book/3399523/14ceb5)
- [Online/Offline](https://en.wikipedia.org/wiki/Online_algorithm)
