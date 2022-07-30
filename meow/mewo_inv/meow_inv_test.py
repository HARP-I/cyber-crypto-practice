from meow_inverse import meow_inverse
from meow.include.meow import meow_hash


def key_print(key: bytes):
    print(f'Key: {key.hex()[:64]}')
    print(f'     {key.hex()[64:128]}')
    print(f'     {key.hex()[128:192]}')
    print(f'     {key.hex()[192:]}')


def invert_test():
    message = b"Xiaohan Ni 202000460071"
    hash_val = b"sdu_cst_20220610"

    re_key = meow_inverse(hash_val, message)
    if meow_hash(re_key, message) == hash_val:
        print('Hash(Key, Message) == Hash_val')
        print(f'Message:  {message.decode()}')
        print(f'Hash_val: {hash_val.decode()}')
        key_print(re_key)
        print()

    # To generate a random key each time
    re_key = meow_inverse(hash_val, message, rand=True)
    if meow_hash(re_key, message) == hash_val:
        print('Hash(Key, Message) == Hash_val')
        print(f'Message: {message.decode()}')
        print(f'Hash_val:    {hash_val.decode()}')
        key_print(re_key)
        print()


if __name__ == '__main__':
    invert_test()
