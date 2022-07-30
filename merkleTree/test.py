import os
import sys

from merkle_rfc6962 import *

# convert bytes list to hex str list
b2h = lambda b: [each.hex() for each in b]
# convert str list to bytes list
s2b = lambda s: [bytes(each, encoding="utf-8") for each in s]


# show all node when generating
# merkle_flag["print"] = True

def main():
    # make list
    # test3=====================
    D = sorted([os.urandom(5) for i in range(100000)])
    # make tree
    root = merkle_tree_hash(D)

    print(f'root: {root.hex()}')
    test_incluison_proof(D, root)
    test_exclusion_proof(D, root)


def test_incluison_proof(D, root):
    # # build inclusion proof
    print("test_incluison_proof----------------------------------------------")
    ele = D[114514 % len(D)]
    inclusion_proof = prove_inclusion(ele, D)
    _, path = inclusion_proof
    print(f'Audit path of {ele.hex()}:\n {b2h(path)}')
    print()
    in_tree = verify_inclusion(ele, inclusion_proof, root, D)
    if in_tree:
        print(f'Pass the inclusion verification:\n'
              f'ele {ele.hex()} in tree {root.hex()}')
    else:
        print(f'Not pass the inclusion verification')
    print()


def test_exclusion_proof(D, root):
    # build exclusion path
    print("test_excluison_proof----------------------------------------------")
    ele = b'zy123giao'
    exclusion_proof = prove_exclusion(ele, D)
    _, left_path, right_path = exclusion_proof
    print(f'Audit paths of pair which both near {ele.hex()}({ele.decode()}):\n  '
          f'Left path:\n'
          f'{b2h(left_path)}\n'
          f'right path:\n'
          f'{b2h(right_path)}')
    print()
    not_in_tree = verify_exclusion(ele, exclusion_proof, root, D)
    if not_in_tree:
        print(f'Pass the exclusion verification:\nele {ele.hex()} not in tree {root.hex()}')
    else:
        print(f'Not pass the exclusion verification')
    print()


if __name__ == '__main__':
    main()

# test1=====================
# lst = [''.join(random.sample('abcdefghijklmnopqrstuvwxyz0123456789', 5)) for i in range(0, 100000)]
# lst.sort()
# D = s2b(lst)

# test2=====================
# lst = ['a', 'b', 'c', 'd', 'e', 'f']
# D = s2b(lst)
