import os

# from gmssl.sm3 import sm3_hash as sm3_hash_func
import time

from pysmx.SM3 import digest as sm3_hash_func

hash_func = lambda msg: sm3_hash_func(msg)[:4]


def search_collisions_birthday_offline(byte_len: int) -> tuple[bytes, bytes, bytes]:
    """
    search collisions use (naive) birthday attack
    insert hash val in a list then sort and detect
    call it a offline approach
    :param byte_len: length of preimage in byte to search collisions
    :return: [preimage1, preimage2, common_hash]
    """

    lst = []
    random_size = 1 << (byte_len << 3)
    for i in range(random_size):
        p = os.urandom(byte_len)  # preimage
        d = hash_func(p)  # digest
        # print(d, p)

        lst.append((d, p))
    lst.sort(key=lambda x: x[0])

    prev_d, prev_p = b'', b''
    for d, p in lst:
        if prev_d == d and prev_p != p:
            return prev_p, p, d
        prev_d, prev_p = d, p
    # search fail
    return b'', b'', b''


def search_collisions_birthday_online(byte_len: int) -> tuple[bytes, bytes, bytes]:
    """
    search collisions use (other) birthday attack
    insert hash val in a set and detect it online
    call it a online approach
    :param byte_len: length of preimage in byte to search collisions
    :return: [preimage1, preimage2, common_hash]
    """
    # hash_func = digest()
    # digest to preimage
    d2p = {}
    while True:  # have it search without stop
        cur_p = os.urandom(byte_len)
        cur_d = hash_func(cur_p)
        # print(cur_d, cur_p)
        old_p = d2p.setdefault(cur_d, cur_p)
        if old_p != cur_p:  # 1. new set 2. old set
            return cur_p, old_p, cur_d
        # else:
        #     print(old_p)


def search_collisions_birthday_iteration(byte_len: int) -> tuple[bytes, bytes, bytes]:
    """
    why bother random
    :param byte_len: length of preimage in byte to search collisions
    :return: [preimage1, preimage2, common_hash]
    """
    # hash_func = digest()
    # digest to preimage
    d2p = {}
    for i in range(1 << (byte_len << 3)):
        # cur_p = os.urandom(byte_len)
        cur_p = i.to_bytes(byte_len, byteorder="little")
        # print(cur_p.hex())
        cur_d = hash_func(cur_p)
        # print(cur_d, cur_p)
        old_p = d2p.setdefault(cur_d, cur_p)
        if old_p != cur_p:  # 1. new set 2. old set
            return cur_p, old_p, cur_d


def main():
    st = time.time()
    res = search_collisions_birthday_online(16)
    # res = search_collisions_birthday_offline(16)
    # res = search_collisions_birthday_iteration(16)
    print(f"{time.time() - st:.3f}s")
    print(res)


if __name__ == '__main__':
    main()

# legacy1
# if cur_digest not in digest2preimage.keys():
#     digest2preimage[cur_digest] = cur_preimage
# elif digest2preimage[cur_digest] != cur_preimage:
#     return digest2preimage[cur_digest], cur_preimage

# legacy2
# if cur_digest in digest2preimage.keys():
#     return digest2preimage[cur_digest], cur_preimage
# else:
#     digest2preimage[cur_digest] = cur_preimage
