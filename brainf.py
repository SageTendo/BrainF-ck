import sys

from api.program import ProgramLoader


def scan_loops():
    sp = -1

    while program.peek():
        curr_index = program.index()
        prog_byte = program.read()

        if prog_byte == '[':
            sp += 1
            stack[sp] = curr_index
        if prog_byte == ']':
            loop_map[stack[sp]] = curr_index
            sp -= 1

    # if sp != -1:
    #     print(f"Unclosed loop '[' at position: {stack[sp]}")
    #     exit(1)


def main(rom_data):
    pointer = 0
    pc = 0
    sp = -1

    # Scan for loops
    scan_loops()

    # Program Execution
    while pc < len(rom_data):
        # Execute instruction
        prog_byte = rom_data[pc]
        if prog_byte == '[':
            if tape[pointer] == 0:
                pc = loop_map[pc] + 1
                continue

            sp += 1
            stack[sp] = pc
        elif prog_byte == ']':
            if tape[pointer] != 0:
                pc = stack[sp] + 1
                continue

            sp -= 1
        elif prog_byte == '>':
            if pointer + 1 < tape_len:
                pointer += 1
            else:
                pointer = 0
        elif prog_byte == '<':
            if pointer > 0:
                pointer -= 1
        elif prog_byte == '+':
            tape[pointer] = (tape[pointer] + 1) % 255
        elif prog_byte == '-':
            tape[pointer] = (tape[pointer] - 1) % 255
        elif prog_byte == '.':
            print(chr(tape[pointer]), end="")
        elif prog_byte == ',':
            while len(input_buffer) > 0:
                if len(input_buffer) > 0:
                    byte = input_buffer.pop(0)
                    tape[pointer] = ord(byte)
                else:
                    str_input = str(input("user input: "))
                    for byte in str_input:
                        input_buffer.append(byte)
        pc += 1
    print()


if __name__ == '__main__':
    # API instances
    program = ProgramLoader()

    loop_map = {}
    stack = [0] * 1024
    tape_len = 30000
    tape = bytearray(tape_len)
    input_buffer = []

    if len(sys.argv) < 2:
        print("No program file provided...")
        sys.exit(1)

    #  Load the program
    prog_file = sys.argv[1]
    data = program.load(prog_file)
    main(data)
