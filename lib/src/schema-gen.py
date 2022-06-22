#
# The C++ Avro generator doesn't support parsing multiple files so quickly hack the seperate avro
# files together
#

import sys

def main() :
    with open(sys.argv[1], 'w', encoding = 'utf-8') as out :
        for i, item in enumerate(sys.argv[2:]):
            with open(item,'r') as input :
                out.write(input.read())

if __name__ == "__main__":
    main()

