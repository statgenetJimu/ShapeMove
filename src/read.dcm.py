#!/usr/local/bin python3
""" 
    File Name: src/read.dcm.py 
    Description: A small code to read a .dcm file (DICOM) into python
    Written by: Maiko Narahara
"""

import sys
import dicom


def main(argv):
    imagefile = argv[1]
    print(imagefile)
    ds = dicom.read_file(imagefile)
    print(ds)
    print("PatientName: ", ds.PatientName)


if __name__ == "__main__":
    main(sys.argv)
