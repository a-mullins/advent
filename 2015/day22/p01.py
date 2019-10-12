#!/usr/bin/env python3
from sys import stdin


class Creature:
    def __init__(self, hp=50, dmg=1, armor=0, mana=0):
        self.hp = hp
        self.dmg = dmg
        self.armor = armor
        self.mana = mana


if __name__ == '__main__':
    for line in stdin:
        if 'Hit Points:' in line:
            boss_hp = int(line.split()[-1])
        if 'Damage:' in line:
            boss_dmg = int(line.split()[-1])
    pc = Creature(hp=50, dmg=0, armor=0, mana=500)
    boss = Creature(boss_hp, boss_dmg)
