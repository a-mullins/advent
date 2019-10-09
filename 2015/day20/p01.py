#!/usr/bin/env python3
from itertools import chain, combinations
from functools import reduce
from operator import mul


def factors(n):
    facs = []
    f = 2
    m = n
    while m > 1 and f < n:
        if m % f == 0:
            facs.append(f)
            m /= f
        else:
            f += 1
    return facs


def powerset(iterable):
    "powerset([1,2,3]) --> () (1,) (2,) (3,) (1,2) (1,3) (2,3) (1,2,3)"
    s = list(iterable)
    return chain.from_iterable(combinations(s, r) for r in range(1, len(s)+1))


def num_presents(house_num):
    facs = factors(house_num)
    divisors = set(reduce(mul, tup) for tup in powerset(facs))
    divisors.update((1, house_num))
    return sum(divisors) * 10


if __name__ == '__main__':
    house_num = 209831
    presents = num_presents(house_num)
    while presents < 29000000:
        house_num += 1
        presents = num_presents(house_num)
        if presents % 1000000 == 0:
            print(f'{house_num}: {presents}')
    print(house_num)
