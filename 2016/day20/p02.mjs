#!/usr/bin/env node
"use strict";
import fs from "node:fs";

const blacklist = new Uint8Array(2**32);
const lines = fs.readFileSync(0, "ascii").trim().split("\n");

for(const line of lines) {
    let start, finish;
    [start, finish] = line.split("-").map(Number).sort((a,b) => a - b);

    for(let i = start; i<= finish; i++) { // (start,finish) inclusive
        blacklist[i] = 1;
    }
}

let sum = 0;
for(let i=0; i<2**32; i++) {
    if(!blacklist[i]) sum++;
}
console.log(sum);
