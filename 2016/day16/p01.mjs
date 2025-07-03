#!/usr/bin/env node
// Copyright (c) 2025 Adam Mullins
//
// Solution for Advent of Code 2016, Day 16, Part 1
//   https://adventofcode.com/2016/day/16
"use strict";
import fs from "node:fs";

// Psuedo random data generator.
function prng(seed, target_len) {
    let a = seed.split("");
    let b = seed.split("").reverse().map(n => Number(n) ^ 1);
    a = a.concat(0, b).join("");
    if( a.length >= target_len ) {
        return a.slice(0, target_len);
    } else {
        return prng(a, target_len);
    }
}

function checksum(data) {
    let a = Array.from(data, Number);
    let partitions = [];
    for(let i = 0; i < data.length; i+=2) {
        partitions.push(a.slice(i, i+2));
    }
    let sum = partitions.map(s => s[0] === s[1] ? 1 : 0).join("");
    if(sum.length % 2) {
        return sum;
    } else {
        return checksum(sum);
    }
}

console.log(checksum(prng(fs.readFileSync(0, "ascii").trim(), 272)));
