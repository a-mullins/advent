#!/usr/bin/env python3
from collections import namedtuple
from itertools import product
from sys import stdin


Creature = namedtuple('Creature', ['name', 'hp', 'dmg', 'dr'],
                      defaults=['MISSINGNO.', 0, 0, 0])
Item = namedtuple('Item', ['cost', 'dmg', 'dr'])


def who_wins(attacker, defender):
    atk_turns = defender.hp / max(attacker.dmg - defender.dr, 1)
    def_turns = attacker.hp / max(defender.dmg - attacker.dr, 1)
    return attacker if atk_turns <= def_turns else defender


def parse_shop_inv(inv):
    weps = []
    armor = []
    rings = []
    cur = weps
    for line in inv:
        parts = line.split()
        if line == '':
            continue
        elif parts[0] == 'Weapons:':
            cur = weps
        elif parts[0] == 'Armor:':
            cur = armor
        elif parts[0] == 'Rings:':
            cur = rings
        else:
            cur.append(Item(*[int(n) for n in parts[-3:]]))
    return (weps, armor, rings)


if __name__ == '__main__':
    inp = [int(line.split()[-1]) for line in stdin]
    boss = Creature('Boss', *inp)

    with open('shop_inv', 'rt') as fp:
        weps, armor, rings = parse_shop_inv((line.strip() for line in fp))

    armor.append(Item(0, 0, 0))  # no armor
    rings.append(Item(0, 0, 0))  # no ring
    rings.append(Item(0, 0, 0))  # no ring

    winning_costs = []
    losing_costs = []
    for loadout in product(weps, armor, rings, rings):
        if loadout[-1] is not loadout[-2]:
            pc = Creature(name='PC',
                          hp=100,
                          dmg=sum(item.dmg for item in loadout),
                          dr=sum(item.dr for item in loadout))
            if who_wins(pc, boss) is pc:
                winning_costs.append(sum(item.cost for item in loadout))
            else:
                losing_costs.append(sum(item.cost for item in loadout))
    print(f"PC won {len(winning_costs)} times and "
          f"lost {len(losing_costs)} times.\n"
          f"Least expensive winning loadout cost {min(winning_costs)} gp.\n"
          f"Most expensive losing loadout cost {max(losing_costs)} gp.")
