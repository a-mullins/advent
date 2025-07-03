#!/usr/bin/env node
// Copyright (c) 2025 Adam Mullins
//
// Solution for Advent of Code 2016, Day 02, Part 1
//   https://adventofcode.com/2016/day/2
"use strict";
import fs from "node:fs";

const blacklist = new Uint8Array(2**32);
const lines = fs.readFileSync(0, "ascii").trim().split("\n");

for(const line of lines) {
    let start, finish;
    [start, finish] = line.split("-").map(Number).sort((a,b) => a - b);

    for(let i = start; i<= finish; i++) { // inclusive
        blacklist[i] = 1;
    }
}

let i = 0;
while(blacklist[i] && i < 2**32) {
    i++;
}
console.log(i);
