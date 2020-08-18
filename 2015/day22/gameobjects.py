#!/usr/bin/env python3
"""Game objects for Advent of Code 2015, day 22."""
from dataclasses import dataclass, field
from typing import List


# ---------------------------------------------------------------------
# SPELLS & EFFECTS
# ---------------------------------------------------------------------

# --[ Base Classes ]---------------------------------------------------
@dataclass
class Spell:
    """Spell base class.

    Spell is a base class from which other more specific spells
    should inherit.

    The actual spell effect should be implemented in the on_cast()
    hook method, whether that is damaging a target directly or adding
    a helpful Effect to the caster.

    Note that the Spell object is _NOT_ responsible for deducting mana
    from its caster. That is handled by Creature.cast()

    attributes:
    name -- spell name.
    cost --  how much mana this spell costs to cast.

    """
    name: str = 'NOP'
    cost: int = 1

    def on_cast(self, caster, target):
        """Hook called when this spell is cast by a Creature."""
        pass


@dataclass
class Effect:
    """Effect base class. Represents ongoing effects

    Other, more concrete Effects should inherit from this class.

    The Effects in question could be buffs that last a certain number
    of turns and then dissipate, or damage-over-time-spells that hurt
    a target every turn.

    An Effect modifies the game world through its hooks, which all
    begin with 'on_'. See their docstrings for more information.

    name: effect name
    timer: how many turns are left before this effect is deleted
    mag: the magnitude of the effect. damage done, hp healed, etc.

    """
    name: str = 'NOP'
    timer: int = 1
    mag: int = 0

    def on_add(self, target):
        """Hook called once when effect is added to a creature."""
        pass

    def on_process(self, target):
        """Hook called every turn."""
        pass

    def on_del(self, target):
        """Hook called once when effect's timer reaches 0 and it is
        removed from a creature."""
        pass


# --[ Specific Spells ]------------------------------------------------
@dataclass
class MagicMissile(Spell):
    """Magic Missile is damaging evocation spell.

    It is a basic spell which instantly and unerringly harms its
    target. The most common version costs 53 mana and does 4 damage.

    Every good wizard should have a copy of Magic Missile in their
    spellbook.

    """
    name: str = 'Magic Missile'
    cost: int = 53
    dmg: int = 4

    def on_cast(self, caster, target):
        target.hp -= self.dmg


@dataclass
class Drain(Spell):
    """Drain damages your opponent while healing yourself.

    Drain is typcially placed in the necromancy school, as it
    unnaturally modifies its targets life-force.

    The most common version does 2 damage and heals you for 2 hit
    points.

    """
    name: str = 'Drain'
    cost: int = 73
    dmg: int = 2
    healing: int = 2

    def on_cast(self, caster, target):
        self.subtract_mana(caster)
        target.hp -= self.dmg
        caster.hp += self.healing


@dataclass
class Shield(Spell):
    """Shield increases the caster's armor for a number of turns.

    Shield is from the abjuration school of magic, which focuses on
    protection, warding, and alarms.

    The most common version of Shield costs 113 mana, has a duration
    of 6 turns, and increases dr by 7.

    """
    name: str = 'Shield'
    cost: int = 113
    dur: int = 6
    mag: int = 7

    @dataclass
    class ShieldEffect(Effect):
        name: str = 'Shield'

        def on_add(self, target):
            target.dr += self.mag

        def on_del(self, target):
            target.dr -= self.mag

    def on_cast(self, caster, target):
        target.add_effect(self.ShieldEffect(timer=self.timer, mag=self.mag))


@dataclass
class Poison(Spell):
    """Poison slowly drains its target's hp over time.

    Poison is usually placed in the Conjuration school of magic
    as the caster creates a poison to inflict on their target, but
    the exact classification is an occasional point of minor debate.

    The common variant of poison costs 173 mana and lasts 6 turns, and
    deals 3 damage to its target while active.

    """
    name: str = 'Poison'
    cost: int = 173
    dur: int = 6
    mag: int = 3

    @dataclass
    class PoisonEffect(Effect):
        name: str = 'Poison'

        def on_process(self, target):
            target.hp -= self.mag

    def on_cast(self, caster, target):
        target.add_effect(self.PoisonEffect(timer=self.dur, mag=self.mag))


@dataclass
class Recharge(Spell):
    """Recharge increases its caster more mana over time.

    Recharge hails from the abjuration school.

    A typical example costs 229 mana, lasts 5 turns and gives its
    target 101 new mana every turn while active.

    """
    name: str = 'Recharge'
    cost: int = 239
    dur: int = 5
    mag: int = 101

    @dataclass
    class RechargeEffect(Effect):
        name: str = 'Recharge'

        def on_process(self, target):
            target.mana += self.mag

    def on_cast(self, caster, target):
        target.add_effect(self.RechargeEffect(timer=self.timer, mag=self.mag))


# ---------------------------------------------------------------------
# CREATURE
# ---------------------------------------------------------------------
@dataclass
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
    name: str = 'MISSINGNO'
    hp: int = 50
    dr: int = 0
    dmg: int = 1
    mana: int = 0
    spellbook: List[Spell] = field(default_factory=list)
    effects: List[Effect] = field(default_factory=list)

    # --[ Effects ]----------------------------------------------------
    def add_effect(self, effect):
        effect.on_add(self)
        self.effects.append(effect)

    def del_effect(self, effect):
        effect.on_del(self)
        self.effects.remove(effect)

    def process_ongoing_effects(self):
        expired = []

        for eff in self.effects:
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
# HELPFUL CONSTS
# ---------------------------------------------------------------------
common_spells = (
    MagicMissile(),
    Drain(),
    Shield(),
    Poison(),
    Recharge()
)
