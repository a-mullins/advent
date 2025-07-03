#!/usr/bin/env node
// Copyright (c) 2025 Adam Mullins
//
// Solution for Advent of Code 2016, Day 01, Part 1
//   https://adventofcode.com/2016/day/1
"use strict";
import fs from "node:fs";


let facing = 0; // 0 = N, 1 = E, 2 = S, 3 = W;
function turnL() {if(--facing < 0) {facing = 3;}}
function turnR() {if(++facing > 3) {facing = 0;}}

let input = fs.readFileSync(0, "ascii").split(", ");
let x = 0;
let y = 0;

for(const elem of input) {
    if(elem[0] === "L") {turnL();}
    if(elem[0] === "R") {turnR();}

    let dist = Number(elem.substr(1));

    // Smaller than switch statement.
    if(facing === 0) {y+=dist;}
    if(facing === 1) {x+=dist;}
    if(facing === 2) {y-=dist;}
    if(facing === 3) {x-=dist;}
};

console.log(Math.abs(x) + Math.abs(y));
