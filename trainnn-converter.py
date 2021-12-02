#!/usr/bin/env python3
from __future__ import annotations
from PIL import Image
import numpy as np
import os, sys

base_img_fn = lambda fp: int(os.path.basename(fp)[0])

def convert_img(img_path: str) -> str:
    img = Image.open(img_path)
    if img.size != (28, 28):
        print(f'resizing: {img_path}')
        img = img.resize((28, 28))
    arr = np.asarray(img)
    if arr.shape[-1] == 3 or arr.shape[-1] == 4:
        fn = lambda rgb: np.mean(rgb[:3])
        arr = np.apply_along_axis(fn, 2, arr)
    arr = arr.reshape(784)
    arr /= 255
    return ' '.join(map(str, arr)) + '\n'

def convert_n(n: int) -> str:
    m = [0.0] * 9
    m[n - 1] = 1.0
    return ' '.join(map(str, m)) + '\n'

def convert_dir(dir_path, data_path, img_fn = base_img_fn) -> None:
    files = list(map(lambda f: os.path.join(dir_path, f), os.listdir(dir_path)))
    N = len(files)
    inputs = map(convert_img, files)
    outputs = map(lambda fp: convert_n(base_img_fn(fp)), files)
    with open(data_path + 'data.in', 'w') as f:
        f.write(f"{N} 2 28 28 1\n")
        for i,input_ in enumerate(inputs):
            # if i % 100 == 0: print(f"done {i}/{N}")
            f.write(input_)
    with open(data_path + 'data.out', 'w') as f:
        f.write(f"{N} 1 9 1 1\n")
        for i,output in enumerate(outputs):
            # if i % 100 == 0: print(f"done {i}/{N}")
            f.write(output)


if __name__ == "__main__":
    args = sys.argv[1:]
    if len(args) != 2:
        sys.stderr.write("Usage: ./trainnn-converter.py directory-path data-prefix\n")
        exit(1)
    convert_dir(*args)

