#!/usr/bin/env python3
from sys import stdin


class Creature:
    """Represents both Henry Case's player character (pc) and the boss.

    Each Creature keeps track of several different attributes.
    These attributes are also constructor keyword args:
    hp        -- number of hitpoints remaining
    dr        -- damage resistance, which is subtracted from incoming damage
    dmg       -- damage inflicted by this creature in a basic attack
    mana      -- remaining mana points, which are used to cast spells
    spellbook -- list of spells that this creature knows and can cast
    effects   -- list of ongoing effects that apply to this creature

    """
    def __init__(self,
                 name='MISSINGNO.', hp=50, dr=0, dmg=1,
                 mana=0, spellbook=[], effects=[]):
        self.name = name

        self.hp = hp
        self.dr = dr
        self.dmg = dmg

        self.mana = mana
        self.spellbook = spellbook
        self.effects = []

    def __repr__(self):
        return f'Creature(name=\'{self.name}\', '\
               f'hp={self.hp}, dmg={self.hp}, '\
               f'dr={self.dr}, mana={self.mana})'

    def __str__(self):
        return f'{self.name}\n'\
               f'\t{self.hp} HP, {self.dr} DR,\n'\
               f'\t{self.dmg} DMG, {self.mana} MANA'

    def apply_effects(self):
        expired = []

        for effect in self.effects:
            effect.fun(self)
            effect.timer -= 1
            if effect.timer <= 0:
                expired.append(effect)

        for elem in expired:
            self.effects.remove(elem)


class Effect:
    def __init__(self, name='NOP', timer=1, fun=lambda _: _):
        self.name = name
        self.timer = timer
        self.fun = fun

    def __repr__(self):
        return f'Effect(name=\'{self.name}\')'


class Spell:
    def __init__(self, name='NOP', cost=1, effect=Effect()):
        self.name = name
        self.cast = cost
        self.effect = effect

    def __repr__(self):
        return f'Spell(name=\'{self.name}\')'


if __name__ == '__main__':
    # Define spells...
    # Magic Missile costs 53 mana. It instantly does 4 damage.
    # Drain costs 73 mana. It instantly does 2 damage and heals you
    #   for 2 hit points.
    # Shield costs 113 mana. It starts an effect that lasts for 6
    #   turns. While it is active, your armor is increased by 7.
    # Poison costs 173 mana. It starts an effect that lasts for 6
    #   turns. At the start of each turn while it is active, it deals
    #   the boss 3 damage.
    # Recharge costs 229 mana. It starts an effect that lasts for 5
    #   turns. At the start of each turn while it is active, it gives
    #   you 101 new mana.
    spells = [
        Spell('Magic Missile', 53),
        Spell('Drain', 73),
        Spell('Shield', 113),
        Spell('Poison', 173),
        Spell('Recharge', 229)]

    for line in stdin:
        if 'Hit Points:' in line:
            boss_hp = int(line.split()[-1])
        if 'Damage:' in line:
            boss_dmg = int(line.split()[-1])
    pc = Creature(hp=50, dmg=0, armor=0, mana=500)
    boss = Creature(boss_hp, boss_dmg)
