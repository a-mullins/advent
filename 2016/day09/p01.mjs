#!/usr/bin/env node
// Copyright (c) 2025 Adam Mullins
//
// Solution for Advent of Code 2016, Day 09, Part 1
//   https://adventofcode.com/2016/day/9
"use strict";
import fs from "node:fs";


const input = fs.readFileSync(0, "ascii").trim();

let count = 0;
for(let i = 0; i < input.length; /* nop */) {
    if(input[i] === "(") {
        let [m, len, rep] = /(\d+)x(\d+)/.exec(input.slice(i+1));
        i += m.length + 2;

        count += input.slice(i, i+Number(len)).repeat(Number(rep)).length;
        i += Number(len);
    } else {
        count++;
        i++;
    }
}

console.log(count);
