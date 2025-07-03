#!/usr/bin/env node
// Copyright (c) 2025 Adam Mullins
//
// Solution for Advent of Code 2016, Day 02, Part 1
//   https://adventofcode.com/2016/day/2
"use strict";
import fs from "node:fs";


const left   = [1, 4, 7];
const right  = [3, 6, 9];
const top    = [1, 2, 3];
const bottom = [7, 8, 9];

const input = fs.
      readFileSync(0, "ascii").
      trim().
      split("\n");

let code = "";
let cur = 5;

for(const line of input) {
    for(const c of line) {
        if(c === "L") {if(!left.includes(cur))   {cur--;}}
        if(c === "R") {if(!right.includes(cur))  {cur++;}}
        if(c === "U") {if(!top.includes(cur))    {cur -= 3;}}
        if(c === "D") {if(!bottom.includes(cur)) {cur += 3;}}
    }
    code += String(cur);
}

console.log(code);
