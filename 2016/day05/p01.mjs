#!/usr/bin/env node
// Copyright (c) 2025 Adam Mullins
//
// Solution for Advent of Code 2016, Day 05, Part 1
//   https://adventofcode.com/2016/day/5
"use strict";
import fs from "node:fs";
import crypto from "node:crypto";


const input = fs.readFileSync(0, "ascii").trim();
let password = "";
let index = 0;
while(password.length < 8) {
    let next = crypto.createHash("md5")
        .update(input + String(index)).digest("hex");
    if(next.slice(0, 5) === "00000") {
        password += next[5];
        console.log(input+String(index), next, password);
    }
    index++;
}
