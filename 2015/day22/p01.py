#!/usr/bin/env python3
from dataclasses import dataclass, field
from io import TextIOBase
from typing import List, Optional
import sys


@dataclass
class Effect:
    name:  str
    timer: int
    mag:   int

    def on_add(self, target, output=None):
        pass

    def on_process(self, target, output=None):
        pass

    def on_del(self, target, output=None):
        pass


@dataclass
class Spell:
    name: str
    cost: int
    mag:  int = 0
    dur:  int = 0

    def on_cast(self, caster, target, output=None):
        pass


@dataclass
class Creature:
    name:       str = 'MISSINGNO.'
    hp:         int = 50
    armor:      int = 0
    dmg:        int = 1
    mana:       int = 500
    mana_spent: int = 0
    _effects:   List[Effect] = field(default_factory=list, repr=False)
    _output:    Optional[TextIOBase] = field(default=None, repr=False)

    def add_effect(self, eff):
        if self._output:
            print(f'{self.name} is now affected by {eff.name}!',
                  file=self._output)
        eff.on_add(self, output=self._output)
        self._effects.append(eff)

    def del_effect(self, eff):
        if self._output:
            print(f'{self.name} is no longer affected by {eff.name}.',
                  file=self._output)
        if eff in self._effects:
            self._effects.remove(eff)
            eff.on_del(self, output=self._output)

    def process_effects(self):
        expired = []
        for eff in self._effects:
            eff.timer -= 1
            eff.on_process(self, output=self._output)
            if eff.timer <= 0:
                expired.append(eff)
        for eff in expired:
            self.del_effect(eff)

    def attack(self, target):
        damage = max(self.dmg - target.armor, 1)
        if self._output:
            print(f'{self.name} attacks {target.name} for '
                  f'{damage} damage!',
                  file=self._output)
        target.hp -= damage

    def cast(self, spell, target):
        if self.mana < spell.cost:
            raise Exception(f'not enough mana to cast {spell.name}')
        self.mana -= spell.cost
        spell.on_cast(caster=self, target=target, output=self._output)


@dataclass
class MagicMissile(Spell):
    name: str = 'Magic Missile'
    cost: int = 53
    mag:  int = 4

    def on_cast(self, caster, target, output=None):
        if output:
            print(f'{caster.name} casts {self.name}, dealing {self.mag} '
                  f'damage!', file=output)
        target.hp -= self.mag


@dataclass
class Drain(Spell):
    name: str = 'Drain'
    cost: int = 73
    mag:  int = 2

    def on_cast(self, caster, target, output=None):
        if output:
            print(f'{caster.name} casts {self.name}, dealing {self.mag} '
                  f'damage, and healing {self.mag} hit points.',
                  file=output)
        target.hp -= self.mag
        caster.hp += self.mag


@dataclass
class Poison(Spell):
    name: str = 'Poison'
    cost: int = 173
    mag:  int = 3
    dur:  int = 6

    class PoisonEffect(Effect):
        def on_process(self, target, output=None):
            if output:
                print(f'{self.name} deals {self.mag} damage to {target.name}; '
                      f'its timer is now {self.timer}.',
                      file=output)
            target.hp -= self.mag

    def on_cast(self, caster, target, output=None):
        if output:
            print(f'{caster.name} casts {self.name}.', file=output)
        target.add_effect(
            self.PoisonEffect('Poison', timer=self.dur, mag=self.mag))


@dataclass
class Recharge(Spell):
    name: str = 'Recharge'
    cost: int = 229
    mag:  int = 101
    dur:  int = 5

    class RechargeEffect(Effect):
        def on_process(self, target, output=None):
            if output:
                print(f'{self.name} provides {self.mag} mana '
                      f'to {target.name}; its timer is now {self.timer}.',
                      file=output)
            target.mana += self.mag

    def on_cast(self, caster, target, output=None):
        if output:
            print(f'{caster.name} casts {self.name}.', file=output)
        target.add_effect(
            self.RechargeEffect('Recharge', timer=self.dur, mag=self.mag))


@dataclass
class Shield(Spell):
    name: str = 'Shield'
    cost: int = 113
    mag:  int = 7
    dur:  int = 6

    class ShieldEffect(Effect):
        def on_add(self, target, output=None):
            if output:
                print(f'A magical shield springs up around {target.name}, '
                      f'faintly glowing with golden light.', file=output)
            target.armor += self.mag

        def on_process(self, target, output=None):
            if output:
                print(f"{target.name}'s shield is still up; "
                      f"its timer is now {self.timer}.",
                      file=output)

        def on_del(self, target, output=None):
            if output:
                print(f"{target.name}'s magical shield dissipates.",
                      file=output)
            target.armor -= self.mag

    def on_cast(self, caster, target, output=None):
        if output:
            print(f'{caster.name} casts {self.name}, '
                  f'increasing armor by {self.mag}',
                  file=output)
        target.add_effect(
            self.ShieldEffect('Shield', timer=self.dur, mag=self.mag))


def check_winner(c1: Creature, c2: Creature) -> str:
    if c1.hp <= 0 and c2.hp <= 0:
        return 'Draw'
    elif c1.hp <= 0 or c2.hp <= 0:
        if c1.hp <= 0:
            return c2.name
        else:
            return c1.name
    else:
        return ''


if __name__ == '__main__':
    pc = Creature(name='Player', hp=10, mana=250, _output=sys.stdout)
    boss = Creature(name='Boss', hp=14, dmg=8, _output=sys.stdout)
    turn = 0
    recharge = Recharge()
    shield = Shield()
    drain = Drain()
    poison = Poison()
    mm = MagicMissile()

    while pc.hp > 0 and boss.hp > 0:
        print('--' +
              (' Boss ' if turn % 2 else ' Player ') +
              'turn --')
        print(f'- Player has {pc.hp} hit points, '
              f'{pc.armor} armor, '
              f'{pc.mana} mana\n'
              f'- Boss has {boss.hp} hit points')

        pc.process_effects()
        boss.process_effects()

        winner = check_winner(pc, boss)
        if winner:
            print(f'{winner} wins!')
            break

        if turn % 2:
            boss.attack(pc)
        else:
            if turn == 0:
                pc.cast(recharge, pc)
            elif turn == 2:
                pc.cast(shield, pc)
            elif turn == 4:
                pc.cast(drain, boss)
            elif turn == 6:
                pc.cast(poison, boss)
            else:
                pc.cast(mm, boss)

        winner = check_winner(pc, boss)
        if winner:
            print(f'{winner} wins!')
            break

        print()
        turn += 1
