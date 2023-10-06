#!/usr/bin/env node
"use strict";
import fs from "node:fs";
import crypto from "node:crypto";
import { fileURLToPath } from "url";
import { Pqueue, TreeNode } from "../util.mjs";


class State {
    static ords = {"first": 0, "second": 1, "third": 2, "fourth": 3};
    static elements = {
         "hydrogen":   "Hy" // 1
        ,"lithium":    "Li" // 3
        ,"cobalt":     "Co" // 27
        ,"ruthenium":  "Ru" // 44
        ,"promethium": "Pm" // 61
        ,"thulium":    "Tm" // 69 (nice)
        ,"polonium":   "Po" // 84
        ,"elerium":    "El" // 115
        ,"dilithium":  "Dt" // 2 * 3 = 6
    };
    elevator_on = 0;
    floors = [];

    constructor() {
        for(const floor of [0, 1, 2, 3]) {this.floors[floor] = new Set();}
    }

    // Return a deep copy of this State object.
    copy() {
        const new_state = new State();
        new_state.elevator_on = this.elevator_on;
        for(let floor = 0; floor < 4; floor++) {
            for(const elem of this.floors[floor]) {
                new_state.floors[floor].add(elem);
            }
        }

        return new_state;
    }

    eq(other) {
        // short circuit as much as possible...
        if(this.elevator_on !== other.elevator_on) {return false;}

        // for sets A & B: A==B ⟺ A⊆B && B⊆A
        for(let floor=0; floor<4; floor++) {
            let set0 = this.floors[floor];
            let set1 = other.floors[floor];

            // Could do something like: [...s0].every(x => s1.has(x)),
            // but let's avoid the overhead of building arrays & lambdas.
            for(const elem of set0) {
                if(!set1.has(elem)) {return false;}
            }
            for(const elem of set1) {
                if(!set0.has(elem)) {return false;}
            }
        }
        return true;
    }

    equiv(other) {
        // states are equivalent if they are
        // 1. valid (we will skip this check, we only generate valid ones)
        // 2. elevators are on the same floor
        // 3. have the same number of generators and chips on each floor
        //    (the types of gen & chip don't matter as long as the floor is valid)
        if(this.elevator_on !== other.elevator_on) {return false;}

        // this method of counting iteratively is uglier than, say, converting
        // the sets to arrays & using .filter().length, but it is significantly
        // (~3 times) faster to avoid allocating arrays and making func calls.
        for(let floor = 0; floor < 4; floor++) {
            let this_num_gens = 0;
            let this_num_chips = 0;
            let other_num_gens = 0;
            let other_num_chips = 0;
            for(const item of this.floors[floor]) {
                if(item[2] === "G") {
                    this_num_gens++;
                } else if(item[2] === "M") {
                    this_num_chips++;
                }
            }
            for(const item of other.floors[floor]) {
                if(item[2] === "G") {
                    other_num_gens++;
                } else if(item[2] === "M") {
                    other_num_chips++;
                }
            }
            if((this_num_gens !== other_num_gens)
               || (this_num_chips !== other_num_chips)) {
                return false;
            }
        }
        return true;
    }

    isValid() {
        let valid = true;
        for(const floor of this.floors) {
            // losing games are when there is a chip on a floor with a
            // generator, unless its matching generator is present.
            //
            // chip strings have the form HyM or LiM.
            // generator strings have the form HyG or LiG.
            const chips = [];
            const gens = [];
            for(const item of floor) {
                if(item[2] === "M") {chips.push(item.slice(0,2));}
                if(item[2] === "G") {gens.push(item.slice(0,2));}
            }
            for(const chip of chips) {
                const same_generator = gens.includes(chip);
                valid &&= (!(gens.length>0) || same_generator);
                //              ↑↑ material implication ↑↑
            }
        }
        return valid;
    }

    isWinning() {
        // A winning state is when there are no items on floors 1-3.
        let count = 0;
        for(let i=0; i<3; i++) {
            count += this.floors[i].size;
        }
        return count === 0;
    }

    // Internal. Doesn't update elevator_on.
    #addItem(item, floor) {
        this.floors[floor].add(item);
    }

    // Internal. Doesn't update elevator_on.
    #delItem(item, floor) {
        this.floors[floor].delete(item);
    }

    // Move one or two items to another floor.
    // Elevator must be used to do so, so elevator_on will be updated.
    // Returns a reference to the instance it was called on.
    moveItems(items, toFloor) {
        for(const item of items) {
            this.#delItem(item, this.elevator_on);
            this.#addItem(item, toFloor);
        }
        this.elevator_on = toFloor;
        return this;
    }

    // Returns an array of all items that can be reached from the elevator.
    // Convenience function.
    reachableItems() {
        return [...this.floors[this.elevator_on]];
    }

    // Graphical representation of game state.
    repr() {
        let s = "";
        for(let i=3; i >= 0; i--) {
            s += `F${i+1} `;
            s += (this.elevator_on == i) ? "E " : "  ";
            let item_count = 0;
            for(const item of [...this.floors[i]].sort()) {
                s += item + " ";
                item_count++;
            }
            for(let j=14-item_count; j > 0; j--) {s += "___ ";}
            s += "\n";
        }
        return s;
    }

    // md5 hash based on game state.
    digest() {
        return crypto
            .createHash("md5")
            .update(JSON.stringify(this, (key, val) =>
                val instanceof Set ? [...val].sort() : val))
            .digest("hex");
    }

    toString() {
        return this.digest();
    }

    parseLine(line) {
        const parts = line
              .toLowerCase()
              .replaceAll(".","")
              .replaceAll(",", "")
              .replaceAll("-"," ")
              .split(" ");
        const floor = State.ords[parts[parts.indexOf("floor") - 1]];
        const chipIdxs = [];
        const generatorIdxs = [];
        for(let i=0; i<parts.length; i++) {
            if(parts[i] === "microchip") {chipIdxs.push(i-2);}
            if(parts[i] === "generator") {generatorIdxs.push(i-1);}
        }

        for(const i of chipIdxs) {
            this.#addItem(State.elements[parts[i]]+"M", floor);
        }
        for(const i of generatorIdxs) {
            this.#addItem(State.elements[parts[i]]+"G", floor);
        }
    }
}


// Given an array, return all 1- and 2-combos of the elements of the array.
// ex: [a, b, c] -> [[a], [b], [c], [ab], [ac], [bc]]
function combinations(xs) {
    const combos = xs.map(elem => [elem]);
    for(let i = 0; i < xs.length; i++) {
        for(let j = i+1; j < xs.length; j++) {
            combos.push([xs[i], xs[j]]);
        }
    }
    return combos;
}


// Return a list of every valid next game state that can be reached from the one
// provided.
//
// nextStates :: State, number -> [State]
function nextStates(state) {
    const nextStates = [];

    // For every combination of reachable items, move that combination to the
    // floor above and floor below, then filter out the invalid states.
    for(const combo of combinations(state.reachableItems())) {
        let otherFloors;
        if(state.elevator_on === 0) { otherFloors = [1]; }
        if(state.elevator_on === 1) { otherFloors = [0, 2]; }
        if(state.elevator_on === 2) { otherFloors = [1, 3]; }
        if(state.elevator_on === 3) { otherFloors = [2]; }
        for(const floor of otherFloors) {
            let next = state.copy();
            next.moveItems(combo, floor);

            if(next.isValid()) {nextStates.push(next);}
        }
    }
    return nextStates;
}


function weight(state) {
    // In the context of A*, determine the heuristic weighting, aka h(n).
    // Higher h(n) means a longer distance to the goal and less desirable.
    //
    // This heurstic (2*num_floor_items[floor] - 3) sometimes underestimates the
    // cost to the goal by a few moves. This is fine since we want an admissable
    // heuristic for A*, and _underestimating_ is admissable. Overestimating is
    // bad. It might be possible to get to the heurstic to match the number of
    // moves more closely by having it take into account which floor the
    // elevator is on but this isn't necessary atm.
    let weight = 0;
    let num_floor_items = state.floors.map(fl => fl.size);
    for(let floor = 0; floor<3; floor++) {
        let one_up_moves; // How many moves to put all items on the next floor.

        if(num_floor_items[floor] === 0) {continue;}
        if(num_floor_items[floor] === 1) {one_up_moves = 2;}
        else {one_up_moves = 2*num_floor_items[floor] - 3;}

        weight += one_up_moves;
        num_floor_items[floor+1] += num_floor_items[floor];
    }
    return weight;
}


// A*.
var explored_nodes;
var pruned;
function search(root) {
    explored_nodes = 0;
    pruned = 0;

    const explored = [];
    const frontier = new Pqueue();
    frontier.push(root, 0);

    while(!frontier.empty()) {
        explored_nodes++;
        if(!(explored_nodes % 8000)) {
            console.log(`\texplored ${explored_nodes} nodes so far, `
                        + `${pruned} pruned, `
                        + `${frontier.length()} in the frontier.`);
        }

        const cur_node = frontier.pop();
        explored.push(cur_node.data);

        if(cur_node.data.isWinning()) {
            return cur_node;
        }

        for(const state of nextStates(cur_node.data)) {
            if(explored.find(elem => elem.equiv(state))) {
                pruned++;
                continue;
            }

            const child = new TreeNode(state);
            cur_node.addChild(child);
            frontier.push(child, weight(state)+child.depth());
        }
    }
    return null;
}


function main() {
    const root = new TreeNode(new State());
    for(const line of fs.readFileSync(0, "ascii").trim().split("\n")) {
        root.data.parseLine(line);
    }

    let node = search(root);

    if(node) {
        console.log(`I found a solution in ${node.depth()} moves and `
                    + `explored ${explored_nodes} nodes.`);
        let stack = [];
        while(node !== null) {
            stack.unshift(node);
            node = node.parent;
        }
        for(const node of stack) {
            console.log(node.data.repr());
        }
    } else {
        console.log("No solution.");
    }
}


if (process.argv[1] === fileURLToPath(import.meta.url)) {
    main();
}
