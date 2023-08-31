#!/usr/bin/env python3
from collections import namedtuple
from sys import stdin

if __name__ == "__main__":
    Instr = namedtuple("Instruction", "opc opr1 opr2", defaults=(None, None))
    pc = 0  # program counter
    registers = {"a": 0, "b": 0}
    text = []

    for line in stdin:
        text.append(Instr(*(part.strip(",") for part in line.split())))

    while pc < len(text):
        instr = text[pc]

        if instr.opc == "hlf":
            registers[instr.opr1] >>= 1
        if instr.opc == "tpl":
            registers[instr.opr1] *= 3
        if instr.opc == "inc":
            registers[instr.opr1] += 1
        if instr.opc == "jmp":
            pc += int(instr.opr1)
            continue
        if instr.opc == "jie":
            if registers[instr.opr1] % 2 == 0:
                pc += int(instr.opr2)
                continue
        if instr.opc == "jio":
            if registers[instr.opr1] == 1:
                pc += int(instr.opr2)
                continue

        pc += 1

    print(registers)
