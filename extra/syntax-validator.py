#!/usr/bin/env python3
# Syntax checker for Epita's special syntactic needs
# Done by: Nicolas Reyland
import sys
from os.path import isfile as file_exists

def print_error(*a, **k) -> None:
    sys.stderr.write(*a, **k)
    sys.stderr.write(" ")
    sys.stderr.flush()

def check_epita_syntax(file_path: str) -> bool:
    # read file content
    with open(file_path, "r") as file_obj:
        lines: list[str] = file_obj.read().split("\n")
    # validate each line
    for line_number,line in enumerate(lines):
        # pass empty lines
        if not line:
            continue
        # line has content
        num_chars: int = len(line)
        last_char: str = line[num_chars - 1]
        # Check the rules
        # Rule 1: no line should be more than 80 chars long
        # Rule 2: no whitespaces at the end of a line
        if num_chars > 80 or last_char == " "  or last_char == "\t":
            print_error(f"line {line_number + 1} is not valid (len: {num_chars} & char: '{last_char}')")
            sys.exit(1)
            return False
    # syntax is valid
    return True

def main() -> None:
    # Check num args
    if len(sys.argv) < 2:
        print_error("You must give at least one argument (file path)")
        sys.exit(1)
    # Check file paths
    file_path_list: list[str] = sys.argv[1:]
    for fp in file_path_list:
        if not file_exists(fp):
            print_error(f'The file "{fp}" does not exit')
            sys.exit(1)
    # Check the files
    for fp in file_path_list:
        if not check_epita_syntax(fp):
            print_error(f'Failed for "{fp}"')
            sys.exit(1)

# main
if __name__ == "__main__":
    main()

