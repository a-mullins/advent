#!/usr/bin/env node
// Copyright (c) 2025 Adam Mullins
//
// Solution for Advent of Code 2016, Day 12, Part 1
//   https://adventofcode.com/2016/day/12
import fs from "node:fs";
import process from "node:process";
import { fileURLToPath } from "node:url";

import { opt, run } from "./common.mjs";


if (process.argv[1] === fileURLToPath(import.meta.url)) {
    if(process.argv.includes("-D")) {var DEBUG = true;}

    const text =
          opt(fs.readFileSync(0, "ascii"))
          .split("\n")
          .filter(line => line.length > 0)
          .map(line => line.split(" "));

    const r = run(text, undefined, DEBUG);

    console.log(
        ["a", "b", "c", "d"].map(label => `r${label}: ${r[label]}`).join(", ")
    );
}
