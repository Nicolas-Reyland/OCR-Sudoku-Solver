#!python3.9
# nixos
import os, sys

if len(sys.argv) != 2:
    print("usage: ./init-header.py file-path.h")
    sys.exit(1)

root_dir = os.getcwd()
arg = os.path.join(root_dir, sys.argv[1])
print(arg)

filename = os.path.basename(arg)

with open(arg, "w") as file:
    h_name = "NN_" + filename.removesuffix(".h").upper() + "_H"
    s = f"""// {filename}

#ifndef {h_name}
#define {h_name}

// code comes here

#endif\n"""

    file.write(s)

