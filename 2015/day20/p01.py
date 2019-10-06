#!/usr/bin/env python3


def divisors(n):
    divs = []
    for f in range(1,n+1):
        if n % f == 0:
            divs.append(f)
    return divs

def factors(n):
    facs = []
    f = 2
    while n > 1:
        if n % f == 0:
            facs.append(f)
            n /= f
        else:
            f += 1
    return facs


if __name__ == '__main__':
    house_num = 1
    presents = 10
    while presents < 29000000:
        house_num += 1
        presents = sum(divisors(house_num))*10
    print(house_num)
