from PIL import Image
import os
import argparse, sys
import psutil

def ask_user(image):
    image.show()

    answer = 0

    while answer <= 0 or answer> 9:
        try:
            answer = int(input("Quel était ce nombre ?"))
        except ValueError:
            answer = 0
    return answer

def etiqueter(image_path, directory_path, H):
    img = Image.open(image_path)


    result = ask_user(img)

    H[result] += 1

    os.rename(image_path, \
            os.path.join(directory_path, \
            str(result) + "_" + str(H[result])))
    for proc in psutil.process_iter():
        if proc.name() == "display":
            proc.kill()

if __name__ == "__main__":
    parser = argparse.ArgumentParser()

    parser.add_argument('--src', help="The source folder where every images to be converted are stored.")
    parser.add_argument('--start', help="You know, you can't have different images that have the same name, so basically it set the minimum of the y value in the name '[number]_y'")
    args = parser.parse_args()
    files = os.listdir(args.src)
    
    if args.src == None or args.start == None:
        print("usage: python3 img-etiqueter.py --src=[folder] --start=int")
        exit()
    
    H = [int(args.start)] * 10

    for file in files:
        etiqueter(os.path.join(args.src,file), args.src, H)
