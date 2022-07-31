import struct
from meow.include.util import aes_sbox, gf_multiply, pxor, psubq
import os


flag_debug = False
debug_print = print if flag_debug else lambda x: x

gf_table = {(a, b): gf_multiply(a, b) for a in (1, 2, 3) for b in range(256)}


# Inverse-inverse mix columns
# i.e. mix columns
def mix_column(col: bytearray):
    a, b, c, d = col
    return (
        gf_table[2, a] ^ gf_table[3, b] ^ gf_table[1, c] ^ gf_table[1, d],
        gf_table[1, a] ^ gf_table[2, b] ^ gf_table[3, c] ^ gf_table[1, d],
        gf_table[1, a] ^ gf_table[1, b] ^ gf_table[2, c] ^ gf_table[3, d],
        gf_table[3, a] ^ gf_table[1, b] ^ gf_table[1, c] ^ gf_table[2, d],
    )


inverse_inverse_mix_column = mix_column


def aesdec_inverse(a: bytearray, b: bytearray):
    # Xor round key.
    for i in range(16):
        a[i] ^= b[i]
    # Inverse-inverse mix columns
    # i.e. mix columns
    for column_index in range(4):
        a[column_index * 4: column_index * 4 + 4] = \
            mix_column(a[column_index * 4: column_index * 4 + 4])
    # Inverse-inverse sbox.
    # i.e. sbox.
    for i in range(16):
        a[i] = aes_sbox[a[i]]
    # Inverse-inverse sbox.
    # i.e. shift rows.
    for row_index in range(4):
        row = a[row_index::4]
        for _ in range(row_index):
            row = row[1:] + row[:1]
        a[row_index::4] = row


def meow_inverse(hash_bytes: bytes, input_bytes: bytes, rand=False):
    """
    # get key from arbitrary hash (16 bytes) and arbitrary message (arbitrary bytes)
    :param hash_bytes:      An arbitrary hash (16 bytes)
    :param input_bytes:     An arbitrary message (arbitrary bytes)
    :param rand:            Randomly
    :return:                key that makes -- meow_hash(key, input_bytes) == hash_bytes
                            And construct only one key from many possible ones
    """
    assert len(hash_bytes) == 16

    # broadcast back to 8 lane
    hash_bytes += (os.urandom(128 - 16) if rand else hash_bytes * 7)

    lanes = [
        bytearray(hash_bytes[i * 16: i * 16 + 16])
        for i in range(8)
    ]

    def get_lane(i): return lanes[i % 8]

    def meow_shuffle_inverse(i):
        debug_print(f"mix_reverse_final_{i}")
        pxor(get_lane(i + 1), get_lane(i + 2))
        psubq(get_lane(i + 5), get_lane(i + 6))
        aesdec_inverse(get_lane(i + 4), get_lane(i + 1))
        pxor(get_lane(i + 4), get_lane(i + 6))
        psubq(get_lane(i + 1), get_lane(i + 5))
        aesdec_inverse(get_lane(i + 0), get_lane(i + 4))

    def meow_mix_reg_inverse(i, reads):
        pxor(get_lane(i + 1), reads[3])
        psubq(get_lane(i + 2), reads[2])
        aesdec_inverse(get_lane(i + 4), get_lane(i + 1))
        pxor(get_lane(i + 4), reads[1])
        psubq(get_lane(i + 6), reads[0])
        aesdec_inverse(get_lane(i + 0), get_lane(i + 4))

    def meow_mix_inverse(i, block):
        if flag_debug:
            debug_print(f'mix_inverse{i}')
        meow_mix_reg_inverse(i, [  # (15, 0, 1, 16) -- not reversed
            block[offset: offset + 16] for offset in (15, 0, 1, 16)
        ])

    def meow_mix_funky_inverse(i, block):
        meow_mix_reg_inverse(i, (
            b"\0" + block[:15], block[:16], block[17:] + block[:1], block[16:],
        ))

    # Inverse of Aggregate
    psubq(lanes[0], lanes[4])
    pxor(lanes[4], lanes[5])
    pxor(lanes[0], lanes[1])
    psubq(lanes[5], lanes[7])
    psubq(lanes[1], lanes[3])
    psubq(lanes[4], lanes[6])
    psubq(lanes[0], lanes[2])

    # Inverse of Finalize
    for i in range(11, -1, -1):
        meow_shuffle_inverse(i)

    # Pad the input to a multiple of 32 bytes; but add a full block of zeros if we're already a multiple.
    original_length = len(input_bytes)
    target_length = ((len(input_bytes) // 32) + 1) * 32
    input_bytes += b"\0" * (target_length - original_length)
    # Cut off the last block, which we will absorb differently.
    input_bytes, tail_block = input_bytes[:-32], input_bytes[-32:]

    # Inversely, Absorb remaining 32-byte blocks
    bound = (len(input_bytes) // 256) * 256
    off = len(input_bytes) - 32
    while off >= bound:
        meow_mix_inverse(2 + off // 32, input_bytes[off: off + 32])
        off -= 32

    # Absorb the message length, then tail block
    message_length_block = struct.pack("<QQQQ", 0, 0, original_length, 0)
    meow_mix_funky_inverse(1, message_length_block)
    meow_mix_funky_inverse(0, tail_block)

    # Absorb all complete 256-byte blocks
    off = bound
    while off >= 256:
        for _ in range(8):
            meow_mix_inverse(off // 32 - 1, input_bytes[off - 32: off])
            off -= 32

    key_bytes = b"".join(lanes)
    return key_bytes


def is_inv(opo, opi, times=1000):
    for i in range(times):
        a = bytearray(os.urandom(16))
        b = bytearray(os.urandom(16))
        a0 = a.copy()
        opo(a, b)
        opi(a, b)
        if a != a0:
            return False
    return True


# def is_op_inv():
#     return is_inv(psubq, paddq) and is_inv(pxor, pxor) and is_inv(aesdec, aesdec_inverse)
