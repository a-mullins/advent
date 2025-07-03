#!/usr/bin/env node
// Copyright (c) 2025 Adam Mullins
//
// Solution for Advent of Code 2016, Day 05, Part 2
//   https://adventofcode.com/2016/day/5#part2
"use strict";
import fs from "node:fs";
import crypto from "node:crypto";


const input = fs.readFileSync(0, "ascii").trim();
const password = Array(8).fill(null);
let num_found = 0;
let index = 0;
while(num_found < 8) {
    let next = crypto.createHash("md5").update(input + String(index))
        .digest("hex");
    if(next.slice(0, 5) === "00000"
       && Number(next[5]) < 8 && !password[next[5]] ) {
        password[next[5]] = next[6];
        console.log(input+String(index), next,
                    password.map( c => c ? c : "_" ).join(""));
        num_found++;
    }
    index++;
}
