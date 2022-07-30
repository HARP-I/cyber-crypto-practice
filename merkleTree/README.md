
# Introduction

Implement a Merkel Tree based on [RFC 6962](https://datatracker.ietf.org/doc/html/rfc6962#section-2.1).

## Brief
To keep integrity of data, you can:
- Store pair of (file, hash)
- What if the hash is modified --> **Hash List**
- HashList need too many computations --> **Hash Tree(Merkel Tree)** 


![Hash Tree](https://upload.wikimedia.org/wikipedia/commons/9/95/Hash_Tree.svg)

> Demonstrating that a leaf node is a part of a given binary hash tree requires computing a number of hashes proportional to **the logarithm of the number of leaf nodes** in the tree. Conversely, in a hash list, the number is proportional to the **number of leaf nodes** itself. 
> A Merkle tree is therefore an efficient example of a cryptographic commitment scheme, in which the root of the tree is seen as a **commitment** and leaf nodes may be **revealed and proven** to be part of the original commitment.
> Computation of Hash doesn't depend on times but length. 


## Descrption
<!-- *Project: Impl Merkle Tree following RFC6962 -->

The following descrption is a summary from the RFC6962.

## Merkle Tree Hash (MTH):
> Hash algorithm: SHA-256 (changable)
> - Input: A list of data entries 
>   - D[n] = {d(0), d(1), ..., d(n-1)}
>   - Sorted: for **non-membership** proof
> - Output:A single 32-byte Merkle Tree Hash.

- Empty hash: 
    `MTH({}) = SHA-256()`
- Leaf hash:
    `MTH({d(0)}) = SHA-256(0x00 || d(0))`
- Recursive hash:
    `MTH(D[n]) = SHA-256(0x01 || MTH(D[0:k]) || MTH(D[k:n]))`
- k -- the largest power of two smaller than n

## Merkle Audit Paths
> Merkle audit path(for a leaf):
> - The shortest **list** of additional nodes (in the Merkle Tree) required to compute the **MTH** for that tree.
> - Membership proof:  If the root computed from the audit path matches the true root

- One-element list (0< m <= n-1=0)
    `PATH(0, {d(0)}) = {}`
- For n > 1
   `PATH(m, D[n]) = PATH(m, D[0:k]) : MTH(D[k:n]) for m < k`
   `PATH(m, D[n]) = PATH(m - k, D[k:n]) : MTH(D[0:k]) for m >= k`
- k -- the largest power of two smaller than n


# Implementation and Test

- First implement `merkle_tree_hash` and `merkle_audit_path` based on RFC6962
- Then implement `prove_inclusion`, `prove_exclusion`, `verify_inclusion`,` verify_exclusion`

##  Construct a Merkle tree with 10w leaf nodes
```python
    # make list
    D = sorted([os.urandom(5) for i in range(100000)])
    # make tree
    root = merkle_tree_hash(D)
```
Print the hash node (Locally):
![](https://s2.loli.net/2022/07/29/Zy3aliz1IkbGQCL.png)
`left / right` is the child of `node`, the order of hash node listed here is the same ordered from down to top (of tree).

To enbale the exclusion proof, `D` must be sorted. 


## Build inclusion proof for specified element

Path(a list of hash) is not appended with info of 'left / right', which means we cannot tell how to hash by only path:

```python
state = hash_func(node_prefix + each + state)
# or
state = hash_func(node_prefix + state + each)
```

To make it clear, add a `idx` of ele (in the bottom list) to proof. 
```python
# Build inclusion proof for specified element
# prove ele in merkle tree of D by giving its path
# Callable[[bytes, list[bytes]], list[bytes]]
def prove_inclusion(ele, D):
    idx = bisect.bisect_left(D, ele)
    if D[idx] != ele: return None
    path = merkle_audit_path(idx, D)
    return idx, path
```

When verifying, make hash based the parity of the `idx`.
- Even means left
- Odd means right

```python
def verify_inclusion(ele, proof, root, D):
    n = len(D)
    idx, path = proof
    state = hash_func(leaf_prefix + ele)
    for each in path:
        if idx == n - 1:  # may delay
            while n & 1:  # delay -- cannot find sibling node
                n = (n >> 1) + (n & 1)
                idx = n - 1
        if idx & 1:  # odd  -- right
            state = hash_func(node_prefix + each + state)
        else:  # even -- left
            state = hash_func(node_prefix + state + each)
        idx = idx >> 1  # to higher layer
        n = (n >> 1) + (n & 1)
    return state == root
```

- Normally the `idx` is updated once per layer. 
- But if the node become the left one with no sibling. Hash should be delayded and `idx` is updated many times in loop until it find a sibling to hash with. 

Result of Test:
Choose a node in the tree randomly: 
![](https://s2.loli.net/2022/07/29/oHVvOfJqkz8nIbB.png)
- Show path of node which is proved
- And the proof passes the verification

## Build exclusion proof for specified element

Similar to inclusion proof, two path in this case.
```python
def prove_exclusion(ele, D):
    idx = bisect.bisect_left(D, ele)
    if D[idx] == D:  # ele in D
        return None
    left_path = merkle_audit_path(idx - 1, D)
    right_path = merkle_audit_path(idx, D)
    return idx - 1, left_path, right_path
```

To verify, just repack the args then call `verify_inclusion` as subroutine.
```python
def verify_exclusion(ele, proof, root, D):
    idx, left_path, right_path = proof
    i = bisect.bisect_left(D, ele)
    left_ele, right_ele = D[i - 1: i + 1]
    return \
        verify_inclusion(left_ele, (idx, left_path), root, D) and \
        verify_inclusion(right_ele, (idx + 1, right_path), root, D)
```

Result of Test:
Choose a true node not in the tree: 
![](https://s2.loli.net/2022/07/29/uPVXTyOw1jcrHsG.png)
- Show two paths of two nodes which are both near the node to be proved
- The proof passes the verification

> ```
> To show all the node in tree, uncomment: (But too large tree make the stdout a little strange(unfix)) 
> `merkle_flag["print"] = ture`


## To do
Add a render of binary hash tree...


# Reference
- [RFC 6962](https://datatracker.ietf.org/doc/html/rfc6962#section-2.1)
- [RFC 9162](https://datatracker.ietf.org/doc/html/rfc9162)
- [Go's example](https://aly.arriqaaq.com/merkle-tree-and-verifiable-data-structures/)
- [wiki - Hash List](https://en.wikipedia.org/wiki/Hash_list)
- [wiki - Merkle Tree](https://en.wikipedia.org/wiki/Merkle_tree)
- [ZFS](https://www.youtube.com/watch?v=Js535LqapFE#t=238)

