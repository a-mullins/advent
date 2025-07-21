#!/usr/bin/env node
// Copyright (c) 2025 Adam Mullins
//
// Solution for Advent of Code 2016, Day 03, Part 1
//   https://adventofcode.com/2016/day/3
"use strict";
import fs from "node:fs";


const input = fs.readFileSync(0, "ascii");

console.log(
    input.
        split("\n").
        map(line => [Number(line.slice( 2,  5)),
                      Number(line.slice( 7, 10)),
                      Number(line.slice(12, 15))]
                      .sort((a, b) => a - b)).
        filter(sides => sides[0] + sides[1] > sides[2]).
        length
);
