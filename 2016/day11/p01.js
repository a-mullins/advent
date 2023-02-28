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
//let tree = require("./tree");
let tree = require("/home/alm/Code/advent/2016/day11/tree");

class State {
    constructor(floors=[[],[],[],[]], cur_floor=0) {
        this.cur_floor = cur_floor;
        this.floors = floors;
        this.sort();
    }

    isValid() {
        let valid = true;
        for(let floor of this.floors) {
            // losing games are when there is a chip on a floor with a
            // generator, unless its matching generator is present.
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

    isWinning() {
        return !this.floors.slice(0,3).some(floor=>floor.length);
    }

    clone() {
        return new State(Array.from(this.floors, floor => [...floor]),
                         this.cur_floor);
    }

    moveItem(item, from_floor, to_floor) {
        let index = this.floors[from_floor].findIndex(x =>
            (x.element == item.element && x.type == item.type));
        if(index >= 0) {
            this.floors[from_floor].splice(index, 1);
            this.floors[to_floor].push(item);
        }
        this.sort();
    }

    eq(other) {
        // Two states are equivalent if the elevator is on the same floor, my empty floors are their empty floors, and every item I have they do as well.
        return JSON.stringify(this) == JSON.stringify(other);
    }

    sort() {
        for(let floor of this.floors) {
            floor.sort((a, b) =>
                a.element.codePointAt(0) - b.element.codePointAt(0) -
                a.type.codePointAt(0) - b.type.codePointAt(0));
        }
    }
}

function combinations(ary) {
    let combos = ary.map(elem => [elem]);
    for(let i = 0; i < ary.length; i++) {
        for(let j = i+1; j < ary.length; j++) {
            combos.push( [ary[i], ary[j]] );
        }
    }
    return combos;
}

function makeNextStates(state) {
    let cur_floor = state.cur_floor;
    let combos = combinations(state.floors[cur_floor]);

    let target_floors = undefined;
    if(cur_floor == 0) { target_floors = [1]; }
    if(cur_floor == 3) { target_floors = [2]; }
    if(cur_floor == 1) {
        // Don't bother going down if the lower floors are empty.
        if(!state.floors[0].length) { target_floors = [2]; }
        else { target_floors = [0, 2]; }
    }
    if(cur_floor == 2) {
        // Don't bother going down if the lower floors are empty.
        if(!(state.floors[0].length || state.floors[1].length)) {
            target_floors = [3];
        } else { target_floors = [1, 3]; }
    }

    let states = [];
    for(let target of target_floors) {
        let singles = [];
        let doubles = [];
        for(let items of combos) {
            let next_state = state.clone();
            next_state.cur_floor = target;
            items.forEach(item => next_state.moveItem(item, cur_floor,
                                                     target));
            if(next_state.isValid()) {
                if(items.length === 1) {
                    singles.push(next_state)
                } else {
                    doubles.push(next_state)
                }
            }
        }
        // TODO
        // if going up, and there is a way to take two items, don't bother
        // with singles. if going down and there is a way to take only one
        // item, down bother with doubles.
        if(target > cur_floor) { // going up
            if(doubles.length > 0) {
                states = states.concat(doubles);
            } else {
                states = states.concat(singles);
            }
        }
        if(target < cur_floor) { // going down
            if(singles.length > 0) {
                states = states.concat(singles);
            } else {
                states = states.concat(doubles);
            }
        }
    }
    return states;
}


var state0   = new State();
var ordinals = [["first", 0], ["second", 1], ["third", 2], ["fourth", 3]];

//for(let line of fs.readFileSync("/home/alm/Code/advent/2016/day11/test", "ascii").trim().split('\n')) {
for(let line of fs.readFileSync(0, "ascii").trim().split('\n')) {
    let parts = line.split(' ');
    //could also just keep an index...
    let floor = ordinals.find(a => a[0] === parts[1])[1];
    while(parts.length > 0) {
        let elem = parts.pop();
        if(elem.includes('microchip')) {
            let chip = {element: parts.pop().split('-')[0], type: 'chip'};
            state0.floors[floor].push(chip);
        }
        if(elem.includes('generator')) {
            let generator = {element: parts.pop(), type: 'generator'};
            state0.floors[floor].push(generator);
        }
    }
} // END input processing

var test = new State();
test.floors = [
    [],
    [],
    [{element: 'hydrogen', type: 'chip'}],
    [{element: 'hydrogen', type: 'generator'}],
];
test.cur_floor = 2;

var n0 = new tree.Node(state0);
var seenStates = [state0];
// var n0 = new tree.Node(test);
// var seenStates = [test];
var node_c = 0;
var win = tree.bfs(n0,
                   node => node.data.isWinning(),
                   node => {
                       let states = makeNextStates(node.data);
                       states = states.filter(cur=>
                           !seenStates.some(other=>other.eq(cur)));
                       seenStates = seenStates.concat(states);

                       let edges = states.map(state =>
                           new tree.Node(state, [], node));

                       node.edges = edges;
                       if( !(++node_c % 500) )
                           console.log(node_c);
                   });

var moves = 0;
var cur = win;
while(cur) {
    moves++;
    cur = cur.parent;
}
console.log(moves - 1);
