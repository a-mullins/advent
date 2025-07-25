#!/usr/bin/env node
// Copyright (c) 2025 Adam Mullins
//
// Solution for Advent of Code 2016, Day 06, Part 1
//   https://adventofcode.com/2016/day/6
"use strict";
import fs from "node:fs";


const codes = fs.readFileSync(0, "ascii").split("\n")
    .map(line => line.split("")).filter(line => line.length > 0);
let counts = Array.from({length: codes[0].length}, () => new Map());
let max_message = "";
let min_message = "";

for(let row = 0; row < codes.length; row++) {
    for(let col = 0; col < counts.length; col++) {
        const letter = codes[row][col];
        counts[col].set(letter, counts[col].get(letter)+1 || 1);
    }
}

for(const col of counts) {
    const max_count = [...col.values()]
          .reduce((prev, cur) => cur > prev ? cur : prev);
    const min_count = [...col.values()]
          .reduce((prev, cur) => cur < prev ? cur : prev);
    max_message += [...col].find(elem => elem[1] === max_count)[0];
    min_message += [...col].find(elem => elem[1] === min_count)[0];
}

console.log(`Max-count Message: ${max_message}\n`
            + `Min-count Message: ${min_message}`);
