#!/usr/local/bin python3
""" 
    File Name: src/read.nd2.py 
    Description: A small code to read a .nd2 file into python
    Written by: Maiko Narahara
"""

import sys
import nd2reader


def main(argv):
    imagefile = argv[1]
    print(imagefile)
    nd2 = nd2reader.Nd2(imagefile)
    print(nd2)
    print(nd2[1])


if __name__ == "__main__":
    main(sys.argv)
