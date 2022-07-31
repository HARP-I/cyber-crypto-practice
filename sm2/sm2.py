import hmac
import random
import struct
import sm2
import os
from pysmx.SM3 import digest as hash_func
from functools import reduce

inverse = lambda x, p: pow(x, p - 2, p)


class SM2EllipticCurve:
    a = 0xFFFFFFFEFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFF00000000FFFFFFFFFFFFFFFC
    b = 0x28E9FA9E9D9F5E344D5A9E4BCF6509A7F39789F515AB8F92DDBCBD414D940E93
    q = 0xFFFFFFFEFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFF00000000FFFFFFFFFFFFFFFF
    n = 0xFFFFFFFEFFFFFFFFFFFFFFFFFFFFFFFF7203DF6B21C6052B53BBF40939D54123
    G = 0x32c4ae2c1f1981195f9904466a39c9948fe30bbff2660be1715a4589334c74c7, \
        0xbc3736a2f4f6779c59bdcee36b692153d0a9877cc62a474002df32e52139f0a0

    is_inf = lambda x: not x
    is_include = lambda x: True
    gen_inf = lambda: None

    @classmethod
    def add(cls, P, Q):
        if cls.is_inf(P):
            return Q
        if cls.is_inf(Q):
            return P
        xp, yp = P
        xq, yq = Q
        if xp == xq:
            if (yp + yq) % cls.q == 0:
                return cls.gen_inf()  # infi
            else:
                return cls.double(P)
        else:  # xp != xq
            lamb = ((yq - yp) * inverse((xq - xp) % cls.q, cls.q)) % cls.q
            xs = (lamb ** 2 - xp - xq) % cls.q
            ys = (lamb * (xp - xs) - yp) % cls.q
            return xs, ys

    @classmethod
    def double(cls, P):
        assert cls.is_include(P)
        if cls.is_inf(P):
            return cls.gen_inf()
        x, y = P
        lamb = ((3 * (x ** 2)) + cls.a) * inverse((2 * y) % cls.q, cls.q) % cls.q
        xr = (lamb ** 2 - 2 * x) % cls.q
        yr = (lamb * (x - xr) - y) & cls.q
        return xr, yr

    @classmethod
    def smul(cls, n: int, P) -> int:
        mask = 1 << 255  # mask to get i-th bit of scalar
        A = cls.gen_inf()
        for i in range(255):
            if n & mask != 0:
                A = cls.add(A, P)
            A = cls.double(A)
            mask >>= 1
        if n & mask:
            A = cls.add(A, P)
        return A


ec = SM2EllipticCurve


def sm2_key_gen(length: int = 32) -> tuple:
    d = os.urandom(32)  # private key
    P_int = ec.smul(int.from_bytes(d, 'big'), ec.G)  # public key
    return d, i2b(P_int)


i2b = lambda x: tuple(each.to_bytes(32, 'big') for each in x)
b2i = lambda x: tuple(int.from_bytes(each, 'big') for each in x)

kdf = lambda Z, klen: (b''.join(hash_func(Z + i.to_bytes(4, 'big'))
                                for i in range(1, (klen - 1) // 32 + 2))[:klen])


def sm2_encrypt(input_bytes: bytes, pk_bytes: tuple[bytes]):
    input_len = len(input_bytes)
    P = b2i(pk_bytes)
    while True:
        while True:  # RNG
            k = int.from_bytes(os.urandom(32), 'big')
            if k < ec.n: break
        kP = ec.smul(k, P)
        x2_bytes, y2_bytes = i2b(kP)
        t = kdf(x2_bytes + y2_bytes, input_len)
        if any(t): break
    kG = ec.smul(k, ec.G)
    x1_bytes, y1_bytes = i2b(kG)
    C1 = x1_bytes + y1_bytes
    C2 = bytes(a ^ b for a, b in zip(input_bytes, t))
    C3 = hash_func(x2_bytes + input_bytes + x2_bytes)
    return C1 + C2 + C3


def sm2_decrypt(input_bytes: bytes, prk_bytes: bytes) -> bytes:
    C1, C2, C3 = input_bytes[:64], input_bytes[64:-32], input_bytes[-32:]
    C1 = C1[:32], C1[32:]
    x2_bytes, y2_bytes = i2b(ec.smul(int.from_bytes(prk_bytes, 'big'), b2i(C1)))
    return bytes(a ^ b for a, b in zip(C2, kdf(x2_bytes + y2_bytes, len(C2))))


def sm2_test():
    prk, pk = sm2_key_gen()
    plaintext = b'giaogiao'
    ciphertext = sm2_encrypt(plaintext, pk)
    print("Encrypt:")
    print(f"Plaintext: {plaintext.hex()}")
    print(f"Ciphertext: {ciphertext.hex()}")
    print("Decrypt:")
    out = sm2_decrypt(ciphertext, prk)
    print(out.hex())
    sm2_encrypt(plaintext, pk)


if __name__ == '__main__':
    sm2_test()
