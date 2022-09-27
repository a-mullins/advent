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

function isStateValid(floor) {
    let valid = true;
    for(let floor of floor) {
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

function isWinningState(state) {
    return !state.slice(0,3).some(floor=>floor.length);
}

function combinations(ary) {
    let combos = [];
    for(let i = 0; i < ary.length; i++) {
        for(let j = i+1; j < ary.length; j++) {
            combos.push( [ary[i], ary[j]] );
        }
    }
    return combos;
}

function cloneState(state) {
    return Array.from(state, floor => [...floor]);
}

function moveItem(state, item, from_floor, to_floor) {
    let index = state[from_floor].findIndex(x =>
        (x.element == item.element && x.type == item.type));
    if(index >= 0) {
        state[from_floor].splice(index, 1);
        state[to_floor].push(item);
    }
}

var state0   = Array.from({length: 4}, () => []);
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
            state0[floor].push(chip);
        }
        if(elem.includes('generator')) {
            let generator = {element: parts.pop(), type: 'generator'};
            state0[floor].push(generator);
        }
    }
} // END input processing


// TODO: Generate all game moves from the current state, filter by
//       those which are valid.
function makeNextStates(cur_state, cur_floor) {
    // Select one, or two items to move up or down a floor.
    // If this results in an invalid state, discard it.
    let combos = cur_state[cur_floor].map(elem=>[elem]);
    combos     = combos.concat(combinations(cur_state[cur_floor]));

    let target_floors = undefined;
    if(cur_floor == 0) { target_floors = [1]; }
    if(cur_floor == 3) { target_floors = [2]; }
    if(cur_floor == 1) {
        // Don't bother going down if the lower floors are empty.
        if(cur_state[0].length == 0) { target_floors = [2]; }
        else { target_floors = [0, 2]; }
    }
    if(cur_floor == 2) {
        // If both floors below this one are empty,
        // don't bother going back down.
        if(!(cur_state[0].length || cur_state[1].length)) {
            target_floors = [3];
        } else { target_floors = [1, 3]; }
    }

    // TODO If two items can go up, don't both with singles.
    // TODO If one item can go down, don't bother with doubles.
    let states = [];
    for(let target of target_floors) {
        for(let items of combos) {
            let next_state = cloneState(cur_state);
            items.forEach(item => moveItem(next_state, item,
                                           cur_floor, target));
            states.push(next_state);
        }
    }
    return states.filter(isStateValid);
}

var n0 = new tree.Node(state0);
// tree.bfs(n0,
//          node => isWinningState(node.data),
//          node => {
             
//         );
