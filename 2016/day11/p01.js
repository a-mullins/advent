#!/usr/bin/env node
"use strict";


// some notes:
// + there are four floors
// + a chip cannot be on the same floor as as an RTG, except for its
//   own matching RTG.
// + All of the RTGs & microchips should be delivered to the fourth floor.
// + There is an elevator to move between floors. It can carry two
//   chips or two RTGs in any combo. It will only move if there is at
//   least one item in in.
// + The elevator stops on every floor to recharge. It stays on the
//   floor long enough for any vulnerable components to be irradiated.
// + So this means that the only 'moves' a player can make is to move
//   one or two items up or down a single floor.

// A particular state of the game includes:
// the content of each of the floors
// which floor the elevator is on
// ideally, losing states are disallowed.


let fs = require("fs");
let tree = require("./tree");


function isStateValid(floors) {
    let valid = true;
    for(let [_, floor] of floors) {
        // losing games are when there is a chip on a floor with a generator,
        // unless its matching generator is present.
        for(let chip of floor.filter(obj => obj.type === 'chip')) {
            let generators = floor.filter(obj => obj.type === 'generator');
            let same_generator = generators.some(gen =>
                gen.element === chip.element);
            //                  ↓↓ material implication ↓↓
            valid = valid && (!(generators.length>0) || same_generator);
        }
    }
    return valid;
}


var floors   = new Map(Array.from({length: 4}, (v, i) => [i, []] ));
var on_floor = 0;
var ordinals = [["first", 0], ["second", 1], ["third", 2], ["fourth", 3]];

for(let line of fs.readFileSync(0, "ascii").trim().split('\n')) {
    let parts = line.split(' ');
    //could also just keep an index...
    let floor = ordinals.find(a => a[0] === parts[1])[1];
    while(parts.length > 0) {
        let elem = parts.pop();
        if(elem.includes('microchip')) {
            let chip = {element: parts.pop().split('-')[0], type: 'chip'};
            floors.get(floor).push(chip);
        }
        if(elem.includes('generator')) {
            let generator = {element: parts.pop(), type: 'generator'};
            floors.get(floor).push(generator);
        }
    }
} // END input processing


// TODO: Generate all game moves from the current state, filter by
//       those which are valid.
