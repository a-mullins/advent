#!/usr/bin/env node
"use strict";
import fs from "node:fs";


const input = fs.readFileSync(0, "ascii").
      split("\n").filter(line => line != "").
      map(line => [Number(line.slice( 2,  5)),
                   Number(line.slice( 7, 10)),
                   Number(line.slice(12, 15))]);

let num_tris = 0;
for(let row = 0; row < input.length; row += 3) {
    for(let col = 0; col < 3; col++) {
        const sides = [input[row  ][col],
                       input[row+1][col],
                       input[row+2][col]].sort((a, b) => a-b);
        if(sides[0] + sides[1] > sides[2]) {
            num_tris++;
        }
    }
}

console.log(num_tris);
