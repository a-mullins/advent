#!/usr/bin/env node
// Copyright (c) 2025 Adam Mullins
//
// Solution for Advent of Code 2016, Day 15, Part 1
//   https://adventofcode.com/2016/day/15
//
// Ugly and brute-force, but effective.
"use strict";
import fs from "node:fs";


const lines = fs.readFileSync(0, "ascii").trim().split("\n");
const discs = [];

for(const line of lines) {
    let mod, start;
    [mod, start] = line.split(" ").map(Number).filter(n => !Number.isNaN(n));
    discs.push({mod: mod, start: start});
}

if(process.argv[1].includes("p02")) {
    discs.push({mod: 11, start: 0});
}

for(let i=0, found=false; !found; i++) {
    for(let j=0; j<discs.length; j++) {
        const disc = discs.at(j);
        if( (disc.start + j + i + 1) % disc.mod != 0 ) {
            found = false;
            break;
        }
        found = true;
    }
    if(found) {
        console.log(i);
    }
}
