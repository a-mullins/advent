#!/usr/bin/env node
// Copyright (c) 2025 Adam Mullins
//
// Solution for Advent of Code 2016, Day 04, Part 1
//   https://adventofcode.com/2016/day/4
"use strict";
import fs from "node:fs";


const input = fs.readFileSync(0, "ascii").
      trim().
      split("\n");
let sum = 0;
for(let line of input) {
    const map = {};
    const their_checksum = line.slice(line.indexOf("[")+1, line.indexOf("]"));
    const sector = Number(/\d+/.exec(line));
    let checksum = "";
    for(const c of line) {
        if(c === "[") { break; }
        if(/[a-z]/.test(c)) {
            map[c] = map[c] + 1 || 1;
        }
    }
    const letter_counts = new Set(Object.values(map).sort((a, b)=>b-a));
    for(const count of letter_counts) {
        checksum += Object.keys(map)
            .filter(key => map[key] === count)
            .sort().join("");
    }
    checksum = checksum.slice(0, 5);
    if(their_checksum === checksum) {
        sum += sector;
    }
}

console.log(sum);
