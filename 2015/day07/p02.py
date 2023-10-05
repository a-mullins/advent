#!/usr/bin/env python3
from p01 import Node
from sys import stdin

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

    graph["b"] = Node("b", graph, value=graph["a"].val())
    for node in graph.values():
        if node._op:
            node._value = None

    print(graph["a"].val())
