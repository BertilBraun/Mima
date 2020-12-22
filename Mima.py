import sys


class Mima:

    def __init__(self):

        self.Akku = 0




def main():
    if len(sys.argv) != 2:
        print('Please enter a file to Execute - python Mima.py "Path/To/YourFile.mima"')
        sys.exit()

    filePath = sys.argv[1]
    print(f'Now executing File: {filePath}')


if __name__ == "__main__":
    main()