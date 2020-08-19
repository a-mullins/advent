#!/usr/bin/env python3
from dataclasses import dataclass, field
from collections import namedtuple
from typing import List, Optional

import gameobjects
from gameobjects import Creature


# GameState = namedtuple('GameState', ['pc', 'boss', 'turn', 'mana_spent',
#                                      'winner'])


@dataclass
class Node:
    Link = namedtuple('Link', ['child', 'weight'])

    pc: Creature
    boss: Creature
    turn: int
    mana_spent: int = 0
    winner: Optional[str] = None

    children: List[Link] = field(default_factory=list)

    def check_winner(self):
        if self.pc.hp <= 0 and self.boss.hp <= 0:
            self.winner = 'Draw'
        else:
            if self.pc.hp <= 0:
                self.winner = 'Boss'
            if self.boss.hp <= 0:
                self.winner = 'Player'

        return self.winner

    def make_next_turns(self):
        # process all ongoing effects on both combatants:
        self.pc.process_ongoing_effects()
        self.boss.process_ongoing_effects()

        if self.check_winner():
            return

        # if it's a boss turn, then the boss only has one possible action:
        # attack the player. therefore, we only need one child.
        if self.turn % 2:
            self.boss.attack(self.pc)
            self.children = []

        # if it's a player turn, then the player has as many choices as
        # they have spells that they have enough mana to cast.
        # so, for each spell who's cost is lesss than pc's current mana,
        # generate a child where the player casts that spell.
        if not self.turn % 2:
            pass


if __name__ == '__main__':
    # starting_pc = Creature('Player', hp=10, mana=250,
    #                        spellbook=[gameobjects.Poison,
    #                                   gameobjects.MagicMissile])
    # starting_boss = Creature('Boss', hp=13)
    n = Node(pc=Creature(hp=0), boss=Creature(), turn=0)
    print(n.check_winner())
