from meow.include.color import render, render2
from meow.include.meow import meow_hash
from meow_symmetry import gen_abc_symmetry_block32, gen_range, meow_symmetry
import os


def key_print(key: bytes):
    print(f'Key: {key.hex()[:64]}')
    print(f'     {key.hex()[64:128]}')
    print(f'     {key.hex()[128:192]}')
    print(f'     {key.hex()[192:]}')


def symmetry32_test():
    message = b"abcdefghabcdefghaijklmnhaijklmnhabcdefghabcdefghaijklmnhaijklmnh"  # len = 32
    key = meow_symmetry(len(message), left_bytes=b'', off_bytes=b'')
    hash_val = meow_hash(key, message)

    key_print(key)
    print(f"Message:  {message.hex()} \t\n({len(message)} bytes, ASCII: {message.decode()})")
    print(f"Hash_val: {render2(hash_val.hex())}")

    num_test = 10
    for i in range(num_test):
        print(f"\nretest {i}")
        message = gen_abc_symmetry_block32(2)
        hash_val = meow_hash(key, message)
        print(f"Message:  {message.hex()}")
        print(f"Hash_val: {render2(hash_val.hex())}")


def symmetry_any_test(length: int):
    try:
        l, r = gen_range(length)
    except AssertionError:
        print("Invalid length (paded to 256n)")
        return
    num_blocks = (r - l) // 32

    # fixed slice
    left_message = os.urandom(l)
    off_message = os.urandom(length - r)
    # changeable slice
    right_message = gen_abc_symmetry_block32(num_blocks)

    message = left_message + right_message + off_message
    key = meow_symmetry(length, left_message, off_message)
    hash_val = meow_hash(key, message)
    key_print(key)

    message_hex = left_message.hex() + render(right_message.hex()) + off_message.hex()
    print(f"Message:  {message_hex} ({length} bytes)")
    print(f"Hash_val: {render2(hash_val.hex())}")

    num_test = 10
    for i in range(num_test):
        print(f"\nretest {i}")
        changed = gen_abc_symmetry_block32(num_blocks)
        message = left_message + changed + off_message
        hash_val = meow_hash(key, message)

        message_str = left_message.hex() + render(changed.hex()) + off_message.hex()
        print(f"Message:  {message_str} ({length} bytes)")
        print(f"Hash_val: {render2(hash_val.hex())}")


if __name__ == '__main__':
    symmetry32_test()
    # symmetry_any_test(233)
