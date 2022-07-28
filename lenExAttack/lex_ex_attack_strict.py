from SM3 import digest as hash_func
import struct

# from gmssl import sm3

H = hash_func


def attack_spec_len(pub, apd, guess_sec_len):
    """
    forge based on pub
    :param pub: public info
    :param apd: data appended bt attacker
    :return: forgery
    """

    def create_pad(len1):
        pad = [0x80]
        reserve1 = len1 % 64 + 1
        range_end = 56 if reserve1 <= 56 else 120
        pad.extend([0] * (range_end - reserve1))
        bit_length = len1 * 8
        pad.extend(struct.pack(">Q", bit_length))
        return bytes(pad)

    pub_msg, pub_hsh= pub
    sec_len = guess_sec_len
    pad = create_pad(sec_len + len(pub_msg))

    msg = pub_msg + pad + apd

    cur_v = int.from_bytes(pub_hsh, byteorder="big")
    cur_v = [(cur_v >> ((7 - i) * 32)) & 0xFFFFFFFF for i in range(8)]

    cur_len = sec_len + len(pub_msg) + len(pad)
    state = cur_v, cur_len
    sig = H(apd, state)
    return msg, sig


def main():
    def instantiate():
        # public info can be obtained by attacker (salt)
        # sec = b"a"
        sec = b"I'm a secret to attacker. ToMakeSecretMuchLongerToMakeSecretMuchLonger" \
              b"ToMakeSecretMuchLongerToMakeSecretMuchLonger" \
              b"ToMakeSecretMuchLongerToMakeSecretMuchLonger" \
              b"ToMakeSecretMuchLongerToMakeSecretMuchLonger" \
              b"ToMakeSecretMuchLongerToMakeSecretMuchLonger" \
              b"ToMakeSecretMuchLongerToMakeSecretMuchLonger" \
              b"... anyway... welcome https://github.com/phanen"
        # public info can be obtained by attacker (password)
        pub_msg = b"I can be obtained by the attacker (attacker has no idea about the length of secret)"
        # pub_msg = b"b"
        pub_hsh = H(sec + pub_msg)
        pub = pub_msg, pub_hsh
        return sec, pub

    is_valid = lambda sec, msg, sig: H(sec + msg) == sig

    # create instance
    sec, pub = instantiate()
    print(f"secret: {sec}")
    print(f"public: {pub}")
    # attacker get the pub info

    for guess_sec_len in range(1000):
        print(f"\nguess sec len: {guess_sec_len}")
        forgery = attack_spec_len(pub, b"data appended by attacker", guess_sec_len)
        flag = is_valid(sec, *forgery)
        print(f"forgery: {forgery}")
        if flag:
            print("Success")
            return


if __name__ == '__main__':
    main()
