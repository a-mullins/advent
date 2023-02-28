#!/usr/bin/env node
"use strict";
const fs = require("fs");


const left   = [1, 4, 7];
const right  = [3, 6, 9];
const top    = [1, 2, 3];
const bottom = [7, 8, 9];

const input = fs.
      readFileSync(0, 'ascii').split('\n').filter(line => line != '');

let code = "";
let cur = 5;

for(const line of input) {
    for(const c of line) {
        if(c === "L") {if(!left.includes(cur))   {cur--;}}
        if(c === "R") {if(!right.includes(cur))  {cur++;}}
        if(c === "U") {if(!top.includes(cur))    {cur -= 3;}}
        if(c === "D") {if(!bottom.includes(cur)) {cur += 3;}}
    }
    code += String(cur);
}

console.log(code);
