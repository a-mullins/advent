#!/usr/bin/env node
"use strict";
const fs = require("fs");


let facing = 0; // 0 = N, 1 = E, 2 = S, 3 = W;
function turnL() {if(--facing < 0) {facing = 3}}
function turnR() {if(++facing > 3) {facing = 0}}

let input = fs.readFileSync(0, "ascii").split(", ");
let dup;
let x = 0;
let y = 0;
const visited = [];

for(const elem of input) {
    if(elem[0] === "L") {turnL();}
    if(elem[0] === "R") {turnR();}

    let dist = Number(elem.substr(1));

    for(let i = 0; i<dist; i++){
        if(facing === 0) {y++;}
        if(facing === 1) {x++;}
        if(facing === 2) {y--;}
        if(facing === 3) {x--;}

        const cur = `${String(x)} ${String(y)}`;
        if(visited.includes(cur)) {
            dup = cur;
            visited.push(cur);
            console.log(
                `dup found: ${cur}\ndist: ${Math.abs(x) +  Math.abs(y)}`);
            break;
        } else {
            visited.push(cur);
        }
    }
    if(dup) {break;}
}
