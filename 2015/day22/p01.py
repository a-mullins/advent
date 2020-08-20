#!/usr/bin/env python3
from collections import namedtuple
from dataclasses import dataclass
from gameobjects import Creature, common_spells
from typing import Any, List

import sys


# @dataclass
# class Node:
#     data: Any = None
#     children: List = []


GameState = namedtuple('GameState', ['pc', 'boss',
                                     'mana_spent', 'winner'])


def check_winner(c1: Creature, c2: Creature):
    if c1.hp <= 0 and c2.hp <= 0:
        return 'Draw'
    else:
        if c1.hp <= 0:
            return c2.name
        if c2.hp <= 0:
            return c1.name

    return ''


if __name__ == '__main__':
    for line in sys.stdin:
        if 'Hit Points:' in line:
            boss_hp = int(line.split()[-1])
        if 'Damage:' in line:
            boss_dmg = int(line.split()[-1])

    pc = Creature(name='Player', hp=50, mana=500,
                  spellbook=common_spells)
    boss = Creature(name='Boss', hp=boss_hp, dmg=boss_dmg)
    min_mana = sys.maxsize
    turn = 0
    root = GameState(pc, boss, 0, '')
    old_leaves = []
    cur_leaves = [root]
    while cur_leaves:
        old_leaves = cur_leaves.copy()
        cur_leaves.clear()
        new_leaves = []

        for state in old_leaves:
            # Start of turn.
            # Process any ongoing effects...
            state.pc.process_ongoing_effects()
            state.boss.process_ongoing_effects()

            # TODO Check for winner.
            state.winner = check_winner(state.pc, state.boss)
            if state.winner:
                print(f'{state.winner} wins on turn {turn}. '
                      f'Player spent {state.mana_spent}.')
                if state.winner == pc.name and \
                   state.mana_spent < min_mana:
                    min_mana = state.mana_spent
                break

            # TODO PC or Boss takes turn.
            if turn % 2 == 0:  # player turn
                for spell in state.pc.spellbook:
                    pass

            else:              # boss turn
                pass
            # TODO Check for winner again.
            assert True

            turn += 1
