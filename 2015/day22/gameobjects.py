#!/usr/bin/env python3
"""Game objects for Advent of Code 2015, day 22."""


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

        self._effects = []

    def __repr__(self):
        return f'Creature(name=\'{self.name}\', '\
               f'hp={self.hp}, dmg={self.hp}, '\
               f'dr={self.dr}, mana={self.mana})'

    def __str__(self):
        return f'{self.name}\n'\
               f'\t{self.hp} HP, {self.dr} DR,\n'\
               f'\t{self.dmg} DMG, {self.mana} MANA'

    # --[ Effects ]----------------------------------------------------
    def add_effect(self, effect):
        effect.on_add(self)
        self._effects.append(effect)

    def del_effect(self, effect):
        self._effects.remove(effect)
        effect.on_del(self)

    def process_ongoing_effects(self):
        expired = []

        for eff in self._effects:
            eff.on_process(self)
            eff.timer -= 1
            if eff.timer <= 0:
                expired.append(eff)

        for eff in expired:
            self.del_effect(eff)

    # --[ Actions ]----------------------------------------------------
    def cast(self, spell, target):
        if self.mana < spell.cost:
            raise Exception(f'{self.name} doesn\'t have enough'
                            f'mana to cast {spell.name}!')
        self.mana -= spell.cost
        spell.on_cast(caster=self, target=target)

    def attack(self, target):
        target.hp -= max(self.dmg - target.dr, 1)


# ---------------------------------------------------------------------
# SPELLS & EFFECTS
# ---------------------------------------------------------------------

# --[ Base Classes ]---------------------------------------------------
class Spell:
    def __init__(self, name='NOP', cost=1):
        self.name = name
        self.cost = cost

    def __repr__(self):
        return f"Spell(name='{self.name}')"

    def on_cast(self, caster, target):
        pass


class Effect:
    def __init__(self, name='NOP', timer=1):
        self.name = name
        self.timer = timer

    def __repr__(self):
        return f"Effect(name='{self.name}'"

    def on_add(self, target):
        pass

    def on_process(self, target):
        pass

    def on_del(self, target):
        pass


# --[ Specific Spells ]------------------------------------------------
class MagicMissile(Spell):
    """Magic Missile costs 53 mana. It instantly does 4 damage."""
    def __init__(self, name='Magic Missile', cost=53, dmg=4):
        self.dmg = dmg
        super().__init__(name=name, cost=cost)

    def on_cast(self, caster, target):
        target.hp -= self.dmg


class Drain(Spell):
    """Drain damages your opponent while healing yourself.

    Drain costs 73 mana.
    It instantly does 2 damage and heals you for 2 hit points.

    """
    def __init__(self, name='Drain', cost=73, dmg=2, healing=2):
        self.dmg = dmg
        self.healing = healing
        super().__init__(name=name, cost=cost)

    def on_cast(self, caster, target):
        self.subtract_mana(caster)
        target.hp -= self.dmg
        caster.hp += self.healing


class Shield(Spell):
    """Shield increases the caster's armor for a number of turns.

    Shield costs 113 mana. It starts an effect that lasts for 6
    turns. While it is active, your armor is increased by 7.

    """
    class ShieldEffect(Effect):
        def __init__(self, name='Shield', timer=6, mag=7):
            self.mag = mag
            super().__init__(name=name, timer=timer)

        def on_add(self, target):
            target.dr += self.mag

        def on_del(self, target):
            target.dr -= self.mag

    def __init__(self, name='Shield', cost=113, dur=6, mag=7):
        self.timer = dur
        self.mag = mag
        super().__init__(name=name, cost=cost)

    def on_cast(self, caster, target):
        target.add_effect(self.ShieldEffect(timer=self.timer, mag=self.mag))


class Poison(Spell):
    """Poison slowly drains its target's hp over time.

    Poison costs 173 mana. It starts an effect that lasts for 6
    turns. At the start of each turn while it is active, it deals 3
    damage to the boss.

    """
    class PoisonEffect(Effect):
        def __init__(self, name='Poison', timer=6, mag=3):
            self.mag = mag
            super().__init__(name=name, timer=timer)

        def on_process(self, target):
            target.hp -= self.mag

    def __init__(self, name='Poison', cost=173, dur=6, mag=3):
        self.dur = dur
        self.mag = mag
        super().__init__(name=name, cost=cost)

    def on_cast(self, caster, target):
        target.add_effect(self.PoisonEffect(timer=self.dur, mag=self.mag))


class Recharge(Spell):
    """Recharge increases its caster more mana over time.

    Recharge costs 229 mana. It starts an effect that lasts for 5
    turns. At the start of each turn while it is active, it gives you
    101 new mana.

    """
    class RechargeEffect(Effect):
        def __init__(self, name='Recharge', timer=5, mag=101):
            self.mag = mag
            super().__init__(name=name, timer=timer)

        def on_process(self, target):
            target.mana += self.mag

    def __init__(self, name='Recharge', cost=229, dur=5, mag=101):
        self.timer = dur
        self.mag = mag
        super().__init__(name=name, cost=cost)

    def on_cast(self, caster, target):
        target.add_effect(self.RechargeEffect(timer=self.timer, mag=self.mag))
