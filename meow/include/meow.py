import struct
from meow.include.util import aes_sbox_inverse, gf_multiply, pxor, paddq

flag_debug = False
debug_print = print if flag_debug else lambda x: x

gf_table = {(a, b): gf_multiply(a, b) for a in (9, 11, 13, 14) for b in range(256)}


def inverse_mix_column(col: bytearray):
    a, b, c, d = col
    return (
        gf_table[14, a] ^ gf_table[11, b] ^ gf_table[13, c] ^ gf_table[9, d],
        gf_table[9, a] ^ gf_table[14, b] ^ gf_table[11, c] ^ gf_table[13, d],
        gf_table[13, a] ^ gf_table[9, b] ^ gf_table[14, c] ^ gf_table[11, d],
        gf_table[11, a] ^ gf_table[13, b] ^ gf_table[9, c] ^ gf_table[14, d],
    )


def aesdec(a: bytearray, b: bytearray):
    # Inverse shift rows.
    for row_index in range(4):
        row = a[row_index::4]
        for _ in range(row_index):
            row = row[-1:] + row[:-1]
        a[row_index::4] = row
    # Inverse sbox.
    for i in range(16):
        a[i] = aes_sbox_inverse[a[i]]
    # Inverse mix columns.
    for column_index in range(4):
        a[column_index * 4: column_index * 4 + 4] = \
            inverse_mix_column(a[column_index * 4: column_index * 4 + 4])
    # Xor round key.
    for i in range(16):
        a[i] ^= b[i]


def meow_hash(key_bytes: bytes, input_bytes: bytes) -> bytes:
    assert len(key_bytes) == 128
    lanes = [
        bytearray(key_bytes[i * 16: i * 16 + 16])
        for i in range(8)
    ]
    get_lane = lambda i: lanes[i % 8]

    def meow_mix_reg(i, reads):
        aesdec(get_lane(i + 0), get_lane(i + 4))
        paddq(get_lane(i + 6), reads[0])
        pxor(get_lane(i + 4), reads[1])
        aesdec(get_lane(i + 4), get_lane(i + 1))
        paddq(get_lane(i + 2), reads[2])
        pxor(get_lane(i + 1), reads[3])

    def meow_mix(i, block):
        debug_print(f'mix_{i}')
        meow_mix_reg(i, [
            block[offset: offset + 16] for offset in (15, 0, 1, 16)
        ])

    def meow_mix_funky(i, block):
        meow_mix_reg(i, (
            b"\0" + block[:15], block[:16], block[17:] + block[:1], block[16:],
        ))

    def meow_shuffle(i):
        debug_print(f"mix_final_{i}")
        aesdec(get_lane(i + 0), get_lane(i + 4))
        paddq(get_lane(i + 1), get_lane(i + 5))
        pxor(get_lane(i + 4), get_lane(i + 6))
        aesdec(get_lane(i + 4), get_lane(i + 1))
        paddq(get_lane(i + 5), get_lane(i + 6))
        pxor(get_lane(i + 1), get_lane(i + 2))

    # Pad the input to a multiple of 32 bytes; but add a full block of zeros if we're already a multiple.
    original_length = len(input_bytes)
    target_length = ((len(input_bytes) // 32) + 1) * 32
    input_bytes += b"\0" * (target_length - original_length)
    # Cut off the last block, which we will absorb differently.
    input_bytes, tail_block = input_bytes[:-32], input_bytes[-32:]

    # Absorb all complete 256-byte blocks.
    off = 0
    while off + 256 <= len(input_bytes):
        for _ in range(8):
            meow_mix(off // 32, input_bytes[off: off + 32])
            off += 32

    # Absorb the tail block and message length in a funky way that's different than all other absorptions.
    meow_mix_funky(0, tail_block)
    message_length_block = struct.pack("<QQQQ", 0, 0, original_length, 0)
    meow_mix_funky(1, message_length_block)

    # Now return to absorbing any remaining 32-byte blocks.
    while off + 32 <= len(input_bytes):
        meow_mix(2 + off // 32, input_bytes[off: off + 32])
        off += 32

    # Finalize.
    for i in range(12):
        meow_shuffle(i)

    # Aggregate the state down into a single lane.
    paddq(lanes[0], lanes[2])
    paddq(lanes[4], lanes[6])
    paddq(lanes[1], lanes[3])
    paddq(lanes[5], lanes[7])
    pxor(lanes[0], lanes[1])
    pxor(lanes[4], lanes[5])
    paddq(lanes[0], lanes[4])

    return lanes[0]
