from hashlib import sha256
from typing import Callable
# from functools import cache
# from functools import reduce
import sys
import bisect


# choose hash algorithm
hash_func: Callable[[bytes], bytes] = lambda x: sha256(x).digest()

# prefix pattern for leaf and node
leaf_prefix = b'\00'
node_prefix = b'\01'

# flag
merkle_flag = {
    "print": False,
    "file": True,
    "cache": False,
}

cache = []


def largest_power_of_2_less_than(n: int):
    ret = 0
    cur = 1  # 2 ** 0
    for i in range(64):
        if cur >= n:  # first include n <=1
            return ret
        ret = cur
        cur <<= 1
    return ret  # 2 ** 64 must > =n


def merkle_tree_hash(D: list[bytes]) -> bytes:
    """MTH"""
    n = len(D)
    flag_s = False

    # MTH({}) = SHA-256()
    if n == 0:  # hash of empty str
        ret = hash_func(b'')
        # ret = b"\xe3\xb0\xc4B\x98\xfc\x1c\x14\x9a\xfb\xf4\xc8\x99o\xb9$'\xaeA\xe4d\x9b\x93L\xa4\x95\x99\x1bxR\xb8U"
    # MTH({d(0)}) = SHA-256(0x00 | | d(0))
    elif n == 1:  # hash of leaf / only entry in list
        ret = hash_func(leaf_prefix + D[0])
    # For n > 1, let k be the largest power of two smaller than n (i.e., k < n <= 2k).
    # MTH(D[n]) = SHA-256(0x01 || MTH(D[0:k]) || MTH(D[k:n]))
    else:
        flag_s = True
        k = largest_power_of_2_less_than(n)
        left = merkle_tree_hash(D[0:k])
        right = merkle_tree_hash(D[k:n])
        ret = hash_func(node_prefix + left + right)

    if merkle_flag["print"]:
        print(f"node:{ret.hex()}")
        # sys.stdout.flush()
        if flag_s:
            print(f"left:{left.hex()}", )
            print(f"right:{right.hex()}")
        # sys.stdout.flush()
        print()
    return ret


def merkle_audit_path(m: int, D: list[bytes]) -> list[bytes]:
    """PATH     or     generate inclusion proof"""
    n = len(D)
    if m < 0 or m >= n:  # n == 0 included
        return []
    #  PATH(0, {d(0)}) = {}
    if n == 1:
        return []

    # For n > 1, let k be the largest power of two smaller than n.
    # PATH(m, D[n]) = PATH(m, D[0:k]) : MTH(D[k:n]) for m < k;
    # PATH(m, D[n]) = PATH(m - k, D[k:n]) : MTH(D[0:k]) for m >= k,
    k = largest_power_of_2_less_than(n)
    if m < k:
        ret = merkle_audit_path(m, D[0:k])
        ret.append(merkle_tree_hash(D[k:n]))
    else:  # m >= k
        ret = merkle_audit_path(m - k, D[k:n])
        ret.append(merkle_tree_hash(D[0:k]))
    return ret


# There's no detail about proof and verification
# Not sure how to implement

# Build inclusion proof for specified element
# prove ele in merkle tree of D by giving its path
# Callable[[bytes, list[bytes]], list[bytes]]
def prove_inclusion(ele, D):
    idx = bisect.bisect_left(D, ele)
    if D[idx] != ele:  # ele not in D
        return None
    path = merkle_audit_path(idx, D)
    return idx, path


# Build exclusion proof for specified element
# (When D are sorted)
# - give inclusion proof of left leaf
# - give inclusion proof of right leaf
# - give inclusion proof of two leaf are neighbor ??
def prove_exclusion(ele, D):
    idx = bisect.bisect_left(D, ele)
    if D[idx] == D:  # ele in D
        return None
    left_path = merkle_audit_path(idx - 1, D)
    right_path = merkle_audit_path(idx, D)
    return idx - 1, left_path, right_path


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


# return root == reduce(lambda x, y: hash_func(node_prefix + x + y), proof, hash_func(leaf_prefix + ele))

def verify_exclusion(ele, proof, root, D):
    idx, left_path, right_path = proof
    # i == idx ??? not sure if recalculation is needed
    i = bisect.bisect_left(D, ele)
    left_ele, right_ele = D[i - 1: i + 1]
    return \
        verify_inclusion(left_ele, (idx, left_path), root, D) and \
        verify_inclusion(right_ele, (idx + 1, right_path), root, D)
