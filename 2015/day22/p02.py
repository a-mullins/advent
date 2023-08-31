#!/usr/bin/env python3
from collections import namedtuple
from dataclasses import dataclass, field
from io import TextIOBase
from typing import List, Optional
from copy import deepcopy
from itertools import chain

# TODO: Cleanup pass


@dataclass
class Spell:
    name: str
    sname: str
    cost: int
    mag: int = 0
    dur: int = 0

    def on_cast(self, caster, target, output=None):
        pass


@dataclass
class Effect:
    name: str
    timer: int
    mag: int

    def on_add(self, target, output=None):
        pass

    def on_process(self, target, output=None):
        pass

    def on_del(self, target, output=None):
        pass


@dataclass
class MagicMissile(Spell):
    name: str = "Magic Missile"
    sname: str = field(default="mm", repr=False)
    cost: int = 53
    mag: int = 4

    def on_cast(self, caster, target, output=None):
        if output:
            print(
                f"{caster.name} casts {self.name}, dealing {self.mag} damage!",
                file=output,
            )
        target.hp -= self.mag


@dataclass
class Drain(Spell):
    name: str = "Drain"
    sname: str = field(default="drain", repr=False)
    cost: int = 73
    mag: int = 2

    def on_cast(self, caster, target, output=None):
        if output:
            print(
                f"{caster.name} casts {self.name}, dealing {self.mag} "
                f"damage, and healing {self.mag} hit points.",
                file=output,
            )
        target.hp -= self.mag
        caster.hp += self.mag


@dataclass
class Shield(Spell):
    name: str = "Shield"
    sname: str = field(default="shield", repr=False)
    cost: int = 113
    mag: int = 7
    dur: int = 6

    class ShieldEffect(Effect):
        def on_add(self, target, output=None):
            if output:
                print(
                    f"A magical shield springs up around {target.name}, "
                    f"faintly glowing with golden light.",
                    file=output,
                )
            target.armor += self.mag

        def on_process(self, target, output=None):
            if output:
                print(
                    f"{target.name}'s shield is still up; "
                    f"its timer is now {self.timer}.",
                    file=output,
                )

        def on_del(self, target, output=None):
            if output:
                print(
                    f"{target.name}'s magical shield dissipates.", file=output
                )
            target.armor -= self.mag

    def on_cast(self, caster, target, output=None):
        if output:
            print(
                f"{caster.name} casts {self.name}, "
                f"increasing armor by {self.mag}.",
                file=output,
            )
        target.add_effect(
            self.ShieldEffect("Shield", timer=self.dur, mag=self.mag)
        )


@dataclass
class Poison(Spell):
    name: str = "Poison"
    sname: str = field(default="poison", repr=False)
    cost: int = 173
    mag: int = 3
    dur: int = 6

    class PoisonEffect(Effect):
        def on_process(self, target, output=None):
            if output:
                print(
                    f"{self.name} deals {self.mag} damage to {target.name}; "
                    f"its timer is now {self.timer}.",
                    file=output,
                )
            target.hp -= self.mag

    def on_cast(self, caster, target, output=None):
        if output:
            print(f"{caster.name} casts {self.name}.", file=output)
        target.add_effect(
            self.PoisonEffect("Poison", timer=self.dur, mag=self.mag)
        )


@dataclass
class Recharge(Spell):
    name: str = "Recharge"
    sname: str = field(default="recharge", repr=False)
    cost: int = 229
    mag: int = 101
    dur: int = 5

    class RechargeEffect(Effect):
        def on_process(self, target, output=None):
            if output:
                print(
                    f"{self.name} provides {self.mag} mana "
                    f"to {target.name}; its timer is now {self.timer}.",
                    file=output,
                )
            target.mana += self.mag

    def on_cast(self, caster, target, output=None):
        if output:
            print(f"{caster.name} casts {self.name}.", file=output)
        target.add_effect(
            self.RechargeEffect("Recharge", timer=self.dur, mag=self.mag)
        )


all_spells = (MagicMissile, Drain, Shield, Poison, Recharge)


#
# END Effects & Spells -------------------------------------------------------
#


@dataclass
class Creature:
    # TODO: remove _output field, or find more elegant solution.
    name: str = "MISSINGNO."
    hp: int = 50
    armor: int = 0
    dmg: int = 1
    mana: int = 500
    mana_spent: int = 0
    spellbook: List[Spell] = field(default_factory=list, repr=False)
    _effects: List[Effect] = field(default_factory=list, repr=False)
    _output: Optional[TextIOBase] = field(default=None, repr=False)

    def add_effect(self, eff):
        if self._output:
            print(
                f"{self.name} is now affected by {eff.name}!",
                file=self._output,
            )
        eff.on_add(self, output=self._output)
        self._effects.append(eff)

    def del_effect(self, eff):
        if self._output:
            print(
                f"{self.name} is no longer affected by {eff.name}.",
                file=self._output,
            )
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
            print(
                f"{self.name} attacks {target.name} for " f"{damage} damage!",
                file=self._output,
            )
        target.hp -= damage

    def cast(self, spell, target):
        if type(spell) is str:
            spell = next(
                x
                for x in self.spellbook
                if x.sname == spell or x.name.lower() == spell
            )
        if self.mana < spell.cost:
            raise Exception(f"not enough mana to cast {spell.name}")
        self.mana -= spell.cost
        self.mana_spent += spell.cost
        spell.on_cast(caster=self, target=target, output=self._output)


# TODO: could used namedlist from PyPI?
#       namedtuple immutablility causes some inelegance.
GameState = namedtuple("GameState", "turn pc boss")


def check_winner(c1: Creature, c2: Creature) -> str:
    if c1.hp <= 0 and c2.hp <= 0:
        return "Draw"
    elif c1.hp <= 0 or c2.hp <= 0:
        if c1.hp <= 0:
            return c2.name
        else:
            return c1.name
    else:
        return ""


if __name__ == "__main__":
    # searching for a solution involves exploring the problem space
    # with a breadth-first search (BFS) as though it were a tree,
    # generating and pruning nodes as we go. A tree per se is never
    # built, but the list of leaf nodes to explore is tracked in the
    # list `to_visit`.
    #
    # TODO: is there a better name for this kind of techinque?

    # initial game state
    to_visit = [
        GameState(
            0,
            Creature(
                name="Player",
                hp=50,
                mana=500,
                spellbook=[spell() for spell in all_spells],
            ),
            Creature(name="Boss", hp=71, dmg=10),
        )
    ]

    cur_winning_mana = 2**63 - 1
    while to_visit:
        gs = to_visit.pop()
        gs.pc.process_effects()
        if not gs.turn % 2:  # player turn
            gs.pc.hp -= 1
        gs.boss.process_effects()

        winner = check_winner(gs.pc, gs.boss)
        if winner:
            if winner == "Player":
                if gs.pc.mana_spent < cur_winning_mana:
                    cur_winning_mana = gs.pc.mana_spent
                print(f"w {gs.pc.mana_spent}", flush=True)
            continue

        if gs.turn % 2:
            gs.boss.attack(gs.pc)
            winner = check_winner(gs.pc, gs.boss)
            if winner:
                if winner == "Player":
                    if gs.pc.mana_spent < cur_winning_mana:
                        cur_winning_mana = gs.pc.mana_spent
                    print(f"w {gs.pc.mana_spent}", flush=True)
            else:
                to_visit.insert(0, GameState(gs.turn + 1, gs.pc, gs.boss))
        else:
            spells_available = [
                spell
                for spell in gs.pc.spellbook
                if spell.cost <= gs.pc.mana
                and spell.name
                not in (
                    x.name for x in chain(gs.pc._effects, gs.boss._effects)
                )
            ]
            for spell in spells_available:
                # note the pre-emptive turn increment.
                next_gs = GameState(
                    gs.turn + 1, deepcopy(gs.pc), deepcopy(gs.boss)
                )
                if spell.sname in ["shield", "recharge"]:
                    next_gs.pc.cast(spell, next_gs.pc)
                else:
                    next_gs.pc.cast(spell, next_gs.boss)

                if check_winner(next_gs.pc, next_gs.boss) == "Player":
                    if next_gs.pc.mana_spent < cur_winning_mana:
                        cur_winning_mana = next_gs.pc.mana_spent
                    print(f"w {next_gs.pc.mana_spent}", flush=True)
                elif next_gs.pc.mana_spent < cur_winning_mana:
                    to_visit.insert(0, next_gs)
    #  END WHILE
