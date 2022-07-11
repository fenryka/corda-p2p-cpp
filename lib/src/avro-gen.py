#
# The C++ Avro generator doesn't support parsing multiple files so quickly hack the seperate avro
# files together
#

import sys


def main():
    with open(sys.argv[1], 'w', encoding='utf-8') as out:
        out.write("[\n")
        for i, item in enumerate(sys.argv[2:]):
            if i:  # print a separator if this isn't the first element
                out.write(',\n')
            with open(item, 'r') as input :
                out.write(input.read())
        out.write("]\n")


if __name__ == "__main__":
    main()

