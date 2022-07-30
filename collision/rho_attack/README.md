# Requirement
```bash
# provide api for sm3
pip install snowland-smx==0.3.1
```

# Result
- Toy demo attack(Reduced sm3 by truncation)
![](https://s2.loli.net/2022/07/27/VmhCrIfbn7cO43y.png)


# Principle

The Rho method is an algorithm for finding collisions that, unlike the
naive birthday attack, requires only a small amount of memory. It works
like this:
1. Given a hash function with n-bit hash values, pick some random hash
value ($H_1$), and define $H_1$ = $H_1'$.
1. Compute $H_2$ = $Hash(H_1)$, and $H_2' =  Hash(Hash(H_1'))$; that is, in the
first case we apply the hash function once, while in the second case
we apply it twice.
1. Iterate the process and compute $H_{i + 1} = Hash(H_i)$, $H_{i + 1}' =
Hash(Hash(H_i'))$, until you reach i such that $H_{i + 1} = H_{i + 1}'$.