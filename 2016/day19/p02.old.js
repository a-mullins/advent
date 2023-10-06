#!/usr/bin/env node
// Solution inspired by /u/aceshades, here:
//     https://old.reddit.com/r/adventofcode/comments/5j4lp1/2016_day_19_solutions/dbdf9mn/
// Quite clever & elegant.
"use strict";
import fs from "node:fs";


let numElves = Number(fs.readFileSync(0, "ascii").trim());
let left = [];
let right = [];
let split = Math.floor(numElves / 2) + 1;

for(let i = 1; i < split; i++) {
    left.push(i);
}
for(let i = split; i <= numElves; i++) {
    right.unshift(i);
}

while(left.length > 0 && right.length > 0) {
    if(left.length > right.length) {
        left.pop();
    } else {
        right.pop();
    }

    right.unshift(left.shift());
    left.push(right.pop());
}

console.log(left[0]);
