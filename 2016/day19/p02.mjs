#!/usr/bin/env node
// Copyright (c) 2025 Adam Mullins
//
// Solution for Advent of Code 2016, Day 19, Part 2
//   https://adventofcode.com/2016/day/19#part2
//
// Inspired by /u/aceshades, here:
//   https://old.reddit.com/r/adventofcode/comments/5j4lp1/2016_day_19_solutions/dbdf9mn/
//
// Required implementing a Uint32Array based Deque, see ../util.js.
// Normal js Arrays were incredibly slow.
"use strict";
import fs from "node:fs";
import { Deque } from "../util.mjs";


let numElves = Number(fs.readFileSync(0, "ascii").trim());
let left = new Deque(numElves);
let right = new Deque(numElves);
let split = Math.floor(numElves / 2) + 1;

for(let i = 1; i < split; i++) {
    left.push(i);
}

for(let i = split; i <= numElves; i++) {
    right.unshift(i);
}

while(!(left.isEmpty() || right.isEmpty())) {
    if(left.length() > right.length()) {
        left.pop();
    } else {
        right.pop();
    }

    right.unshift(left.shift());
    left.push(right.pop());
}

console.log(left.pop());
