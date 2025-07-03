#!/usr/bin/env python3
# Copyright (c) 2025 Adam Mullins
#
# Advent of Code 2015, Day 07, Part 1
#   https://adventofcode.com/2015/day/7
# TODO: extract common elemnts of p01 & p02?
import operator as opr
from sys import stdin


class Node:
    """A node in a graph.

    For this puzzle a node can represent a gate and its output
    line. Note that some gates are actually just constants or
    pass-thrus (ex buffers/roms in digital logic).

    Every node has a label, an operator, and between 0, 1, or 2
    children. The children represent gate inputs. Nodes derive their
    value by applying their operator to the values of their
    children. The value is initialized to None before calculation.
    The value is memoized and stored to avoid preforming the same
    calculations over and over.

    Example:
    Parsing the line 'x AND y -> d' would create a Node
    instance with a label 'd' and operater AND. lchild would be a
    refernce to object 'x', and rchild would be a reference to object
    'y'.
    """

    OPERATORS = {
        "NOT": [1, lambda x: 65535 + 1 + ~x],
        "AND": [2, opr.and_],
        "OR": [2, opr.or_],
        "LSHIFT": [2, opr.lshift],
        "RSHIFT": [2, opr.rshift],
        "BUFFER": [1, lambda lchild_val: lchild_val],
    }

    def __init__(
        self, label, graph, op=None, lchild=None, rchild=None, value=None
    ):
        self.label = label
        self.graph = graph
        self.lchild = lchild
        self.rchild = rchild
        self._value = value
        if op:
            self._op = self.OPERATORS[op]
        else:
            self._op = None

    def __str__(self):
        return (
            f"Node({self.label}, op={self._op}, "
            f"lchild={self.lchild}, "
            f"rchild={self.rchild}, "
            f"value={self._value})"
        )

    def val(self):
        if self._value is None:
            if isinstance(self.lchild, int):
                larg = self.lchild
            else:
                larg = self.graph[self.lchild].val()
            if isinstance(self.rchild, int):
                rarg = self.rchild
            elif self.rchild:
                rarg = self.graph[self.rchild].val()
            if self._op[0] == 1:
                self._value = self._op[1](larg)
            if self._op[0] == 2:
                self._value = self._op[1](larg, rarg)

        return self._value


if __name__ == "__main__":
    graph = {}
    lines = stdin.readlines()

    for line in lines:
        lhs, rhs = line.split("->")
        lbl = rhs.strip()
        parts = [int(s) if s.isdecimal() else s for s in lhs.split()]

        if len(parts) == 1:
            if lhs[0].isdecimal():  # eg 123 -> x
                graph[lbl] = Node(lbl, graph, value=parts[0])
            else:  # eg x -> a
                graph[lbl] = Node(lbl, graph, op="BUFFER", lchild=parts[0])
        if len(parts) == 2:
            graph[lbl] = Node(lbl, graph, op=parts[0], lchild=parts[1])
        if len(parts) == 3:
            graph[lbl] = Node(
                lbl, graph, op=parts[1], lchild=parts[0], rchild=parts[2]
            )

    node = graph["a"]
    print(f"{node.label} -> {node.val()}")
