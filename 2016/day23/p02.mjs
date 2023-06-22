#!/usr/bin/env node
import fs from "node:fs";
import process from "node:process";

import { opt, run } from "./common.mjs";


if(process.argv.includes("-D")) {var DEBUG = true;}
const text = opt(fs.readFileSync(0, "ascii"))
      .split("\n")
      .filter(line => line.length > 0)
      .map(line => line.split(" "));
const r = run(text, {"a": 12}, DEBUG);
console.log(
    ["a", "b", "c", "d"].map(label => `r${label}: ${r[label]}`).join(", "));
