from pprint import pprint
from os.path import join
from os import walk
from sys import argv


def print_help(err=0):
    print("Usage:\npython3", argv[0], "[linux driver path]")
    print("If you want some specific device type, please input that\ndevice type drivers path as the linux driver path argument")
    exit()


if len(argv) != 2:
    print("Incorrenct number of arguments!")
    print_help()

sd = {}

#print("Checking files from", argv[1])

for root, subdirs, files in walk(argv[1], onerror=print_help):
    d = {}
    for f_name in files:
        #print("Checking file", join(root, f_name), flush=True)
        if f_name.endswith(".c"):
            with open(join(root, f_name), 'r') as f:
                f_txt = f.read()
                pos = 1
                while pos > 0:
                    pos = f_txt.find(".compatible", pos) + 1
                    if pos > 0:
                        try:
                            comp_dev = f_txt[pos:].split("\"")[1]
                        except IndexError:
                            comp_dev = "Unidentified"

                        d.setdefault(f_name, []).append(comp_dev)
            if not f_name in d:
                d[f_name] = ["Unidentified"]
    if d:
        sd[root.split("drivers/")[1]] = d

print("\n---------------------------------\nDrivers and their compatabilities:")
pprint(sd)
