#!/usr/bin/env node
"use strict";
import fs from "node:fs";


const nodes = [];
const lines = fs.readFileSync("input", "ascii").trim().split("\n").splice(2);
for(const line of lines) {
    const parts = line.replace(/\s+/g, " ").split(" ");

    nodes.push({name: parts[0],
                x: Number(parts[0].match(/x(\d+)/)[1]),
                y: Number(parts[0].match(/y(\d+)/)[1]),
                size: Number(parts[1].match(/\d+/g)[0]),
                used: Number(parts[2].match(/\d+/g)[0]),
                avail: Number(parts[3].match(/\d+/g)[0])});
}

let count = 0;
for(let i = 0; i < nodes.length; i++) {
    for(let j = 0; j < nodes.length; j++) {
        if(i === j) { continue; }
        if(nodes[i].used !== 0 && (nodes[i].used < nodes[j].avail)) {
            count++;
        }
    }
}

console.log(count);
