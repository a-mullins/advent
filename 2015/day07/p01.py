#!/usr/bin/env python3
import operator as opr

from sys import stdin


class Node:
    """A node in a graph.

    In the context of Advent of Code, it can be thought of as a gate
    and its output wire combined into one object. Note that some
    'gates' are actually just constants or pass-thrus (eg buffers in
    digital logic circuits).

    Every node has a label, an operator, and between 0, 1, or 2
    children. Nodes also have a value which is derived from its children
    and its operator, although it begins as None until it is
    calculated (the value is memoized and stored to avoid preforming
    the same calculations over and over).

    Example:
    Parsing the line 'x AND y -> d' would create a Node
    instance with a label 'd' and operater AND. lchild would be a
    refence to object 'x', and rchild would be a reference to object
    'y'.
    """
    OPERATORS = {'NOT': [1, lambda x: 65535 + 1 + ~x],
                 'AND': [2, opr.and_],
                 'OR': [2, opr.or_],
                 'LSHFT': [2, opr.lshift],
                 'RSHIFT': [2, opr.rshift]}

    def __init__(self, label, op=None,
                 lchild=None, rchild=None, value=None):
        self.label = label
        self.lchild = lchild
        self.rchild = rchild
        self._value = value
        if op:
            self._op = self.OPERATORS[op]
        else:
            self._op = None

    def val(self):
        if not self._value:
            if self._op[0] == 1:
                self._value = self._op[1](self.lchild.val())
            if self._op[0] == 2:
                self._value = self._op[1](self.lchild.val(), self.rchild.val())

        return self._value


if __name__ == '__main__':
    wires = {}
    lines = stdin.readlines()

    print(f'lines: {lines}')
    for line in lines:
        print(f'line: {line}')
        lhs, rhs = line.split('->')
        lbl = rhs.strip()
        parts = lhs.split()
        print(f'rhs: {rhs}, lhs: {lhs}')

        if len(parts) == 1:
            if lhs[0].isdecimal():  # eg 123 -> x
                wires[lbl] = Node(lbl, value=123)
            else:                   # eg x -> a
                wires[lbl] = Node(lbl, lchild=wires[parts[0]])

    print(wires)
