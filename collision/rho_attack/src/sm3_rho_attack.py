import os
import time

from pysmx.SM3 import digest as hash_func


def Rp(hash_val: bytes, byte_len: int) -> bytes:
    """
    pad or cut hash value to byte_len
    :param byte_len:
    :return: byte_len bytes' preimage to next round hash
    """
    cur_len = len(hash_val)
    if cur_len <= byte_len:
        hash_val += (b'\0' * (byte_len - cur_len))
    else:  # cut
        hash_val = hash_val[:byte_len]
    return hash_val


def rho_attack(byte_len: int) -> tuple[bytes, bytes, bytes]:
    # R = lambda hash_val: Rp(hash_val, byte_len)
    R = lambda x: x
    H = lambda msg: hash_func(msg)[:2]
    while True:
        start = os.urandom(byte_len)
        # for i in range(1 << (byte_len << 3)):
        #     start =i.to_bytes(byte_len, byteorder="big")
        #  init
        # p -- preimage, d -- digest
        slow_p = R(start)
        slow_d = H(slow_p)
        # fast_p = R(H(R(start)))
        fast_p = R(slow_d)
        fast_d = H(fast_p)

        while fast_d != slow_d:
            # print(fast_d.hex(), slow_d.hex())
            # print(fast_p.hex(), slow_p.hex())
            slow_p = R(slow_d)
            slow_d = H(slow_p)
            fast_p = R(H(R(fast_d)))
            fast_d = H(fast_p)
        if fast_p != slow_p:
            return fast_p, slow_p, fast_d
        else:
            print(fast_p, slow_p, fast_d)
    return b'', b'', b''


def rho_attack_(byte_len: int) -> tuple[bytes, bytes, bytes]:
    # R = lambda hash_val: Rp(hash_val, byte_len)
    R = lambda x: x
    H = lambda msg: hash_func(msg)[:3]
    while True:
        start = os.urandom(byte_len)
        # for i in range(1 << (byte_len << 3)):
        #     start =i.to_bytes(byte_len, byteorder="big")
        #  init
        # p -- preimage, d -- digest
        slow_p = R(start)
        slow_d = H(slow_p)
        # fast_p = R(H(R(start)))
        fast_p = R(slow_d)
        fast_d = H(fast_p)

        while fast_d != slow_d:
            # print(fast_d.hex(), slow_d.hex())
            # print(fast_p.hex(), slow_p.hex())
            slow_p = R(slow_d)
            slow_d = H(slow_p)
            fast_p = R(H(R(fast_d)))
            fast_d = H(fast_p)
        if fast_p != slow_p:
            return fast_p, slow_p, fast_d
        else:
            slow_p = R(slow_d)
            slow_d = H(slow_p)
            fast_p = R(H(R(fast_d)))
            fast_d = H(fast_p)
            cnt = 1
        while fast_d != slow_d:
            slow_p = R(slow_d)
            slow_d = H(slow_p)
            fast_p = R(H(R(fast_d)))
            fast_d = H(fast_p)
            cnt += 1
        
        print(fast_p, slow_p, fast_d)
    return b'', b'', b''


def main():
    start = time.time()
    print(rho_attack(16))
    print(f"{time.time() - start:.3f}s")


if __name__ == '__main__':
    main()
