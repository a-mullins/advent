#!/usr/bin/env node
// Copyright (c) 2025 Adam Mullins
//
// Solution for Advent of Code 2016, Day 09, Part 2
//   https://adventofcode.com/2016/day/9#part2
"use strict";
import fs from "node:fs";


function decomp_len(s) {
    let m = /\((\d+)x(\d+)\)/.exec(s);
    if( !m ) { // no match
        return s.length;
    }
    let [text, run, rep] = m;
    let i = m.index;
    return Number(s.slice(0, i).length)
        + decomp_len(
            s.slice(i+text.length, i+text.length+Number(run)).repeat(rep)
          )
        + decomp_len(
            s.slice(i+text.length + Number(run))
          );
}


let input = fs.readFileSync(0, "ascii").trim();
console.log(decomp_len(input));
