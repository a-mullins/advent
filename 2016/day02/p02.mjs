#!/usr/bin/env node
// Copyright (c) 2025 Adam Mullins
//
// Solution for Advent of Code 2016, Day 02, Part 2
//   https://adventofcode.com/2016/day/2#part2
"use strict";
import fs from "node:fs";


const keypad = ["       ",
                "   1   ",
                "  234  ",
                " 56789 ",
                "  ABC  ",
                "   D   ",
                "       "];
let row = 3;
let col = 1;
let code = "";

const input = fs.
      readFileSync(0, "ascii").
      trim().
      split("\n");

for(const line of input) {
    for(const c of line) {
        let tmp_row = row,
            tmp_col = col;
        if(c === "L") {--tmp_col;}
        if(c === "R") {++tmp_col;}
        if(c === "U") {--tmp_row;}
        if(c === "D") {++tmp_row;}
        if(keypad[tmp_row][tmp_col] !== " ") {
            row = tmp_row;
            col = tmp_col;
        }
    }
    code += keypad[row][col];
}

console.log(code);
