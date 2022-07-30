import struct
import random
import string
import os
from meow.include.util import pxor, psubq
from meow.mewo_inv.meow_inverse import aesdec_inverse


def gen_range(length: int):
    """ Generate index range of changeable message"""
    end32 = length & (-32)  # ~0b11111
    # total len after zero-pad must not be the multiple of 256
    assert end32 & 0xff != 0, "Total len after zero-pad must not be the multiple of 256, otherwise there'll no" \
                              "blocks after absorbing len block, meaning the final hash will be fixed "
    end256 = length & (-256)
    return end256, end32


def gen_abc_symmetry_block32(num=1, printable=False):
    ret = b''
    def prand(length): return "".join(random.sample(
        string.printable[:-2], length)).encode(encoding="ascii")
    rand = prand if printable else os.urandom
    for _ in range(num):
        a = rand(8)
        b = rand(6)
        cur = (a * 2) + ((a[:1] + b + a[-1:]) * 2)
        ret += cur
    return ret


def meow_symmetry(length: int, left_bytes: bytes, off_bytes: bytes, rand=False):
    """
    # Generate a key for arbitrary hash (16 bytes) and arbitrary message (arbitrary bytes)
    :param length:         Length of origin message
    :param left_bytes:     complete 256 bytes blocks part of origin message
    :param off_bytes:      Last 32 block (len < 32) of origin message
    :param rand            Use broadcast or randomize to make a middle state
    :return:               A key that makes meow_hash(key, message) symmetric
                           And construct only one key from many possible ones
    """

    # total len after zero-pad must not be the multiple of 256
    # complete 256 blocks
    assert len(left_bytes) & 0xff == 0 and len(
        off_bytes) < 32, "Invalid params"

    # broadcast or randomize
    mid_state = b"".join([os.urandom(8) * 2 for _ in range(8)]
                         ) if rand else (b"a1b2c3d4" * 2) * 8

    lanes = [
        bytearray(mid_state[i * 16: i * 16 + 16])
        for i in range(8)
    ]

    def get_lane(i): return lanes[i % 8]

    def meow_mix_reg_inverse(i, reads):
        pxor(get_lane(i + 1), reads[3])
        psubq(get_lane(i + 2), reads[2])
        aesdec_inverse(get_lane(i + 4), get_lane(i + 1))
        pxor(get_lane(i + 4), reads[1])
        psubq(get_lane(i + 6), reads[0])
        aesdec_inverse(get_lane(i + 0), get_lane(i + 4))

    def meow_mix_inverse(i, block):
        meow_mix_reg_inverse(i, [  # (15, 0, 1, 16) -- not reversed
            block[offset: offset + 16] for offset in (15, 0, 1, 16)
        ])

    def meow_mix_funky_inverse(i, block):
        meow_mix_reg_inverse(i, (
            b"\0" + block[:15], block[:16], block[17:] + block[:1], block[16:],
        ))

    # Absorb the message length, then tail block
    tail_block = off_bytes + b'\0' * (32 - len(off_bytes))
    message_length_block = struct.pack("<QQQQ", 0, 0, length, 0)
    meow_mix_funky_inverse(1, message_length_block)
    meow_mix_funky_inverse(0, tail_block)

    # Absorb all complete 256-byte blocks
    off = len(left_bytes)
    while off >= 256:
        for _ in range(8):
            meow_mix_inverse(off // 32 - 1, left_bytes[off - 32: off])
            off -= 32

    key_bytes = b"".join(lanes)
    return key_bytes
