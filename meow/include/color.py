class Color:
    HEADER = '\033[95m'
    OKBLUE = '\033[94m'
    OKCYAN = '\033[96m'
    OKGREEN = '\033[92m'
    WARNING = '\033[93m'
    FAIL = '\033[91m'
    ENDC = '\033[0m'
    BOLD = '\033[1m'
    UNDERLINE = '\033[4m'


cl = Color
render = lambda x: cl.OKBLUE + x + cl.ENDC
render2 = lambda x: cl.OKBLUE + x[:len(x) >> 1] + cl.ENDC + cl.WARNING + x[len(x) >> 1:] + cl.ENDC
