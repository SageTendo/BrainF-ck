import re


class ProgramLoader:
    _data = None
    _pointer = None

    def __init__(self):
        self._data = []
        self._pointer = 0
        self._loops = []

    def load(self, program_file):
        with open(program_file, 'r') as f:
            line = f.readline()
            while len(line):
                # skip comments
                if re.search(r"\w+", line):
                    line = f.readline()

                for byte in line:
                    if byte in [">", "<", ".", ",", "+", "-", "[", "]"]:
                        self._data.append(byte)
                line = f.readline()
        return self._data

    def peek(self):
        if self._pointer < len(self._data):
            return self._data[self._pointer]
        return None

    def read(self):
        if self.peek() is not None:
            byte = self._data[self._pointer]
            self._pointer += 1
            return byte
        raise Exception("No more program bytes to read...")

    def index(self):
        return self._pointer
