#!/usr/bin/env node
"use strict";
const fs = require("fs");


let facing = 0; // 0 = N, 1 = E, 2 = S, 3 = W;
function turnL() {if(--facing < 0) {facing = 3}}
function turnR() {if(++facing > 3) {facing = 0}}

let input = fs.readFileSync(0, "ascii").split(", ");
let x = 0;
let y = 0;

for(const elem of input) {
    if(elem[0] === "L") {turnL();}
    if(elem[0] === "R") {turnR();}

    let dist = Number(elem.substr(1));

    // Smaller than switch statement.
    if(facing === 0) {y+=dist;}
    if(facing === 1) {x+=dist;}
    if(facing === 2) {y-=dist;}
    if(facing === 3) {x-=dist;}
};

console.log(Math.abs(x) + Math.abs(y));
