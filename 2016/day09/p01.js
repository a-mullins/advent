#!/usr/bin/env node
"use strict";
const fs = require("fs");


const input = fs.readFileSync(0, "ascii").trim();

let count = 0;
for(let i = 0; i < input.length; /* nop */) {
    if(input[i] === "(") {
        let [m, len, rep] = /(\d+)x(\d+)/.exec(input.slice(i+1));
        i += m.length + 2;

        count += input.slice(i, i+Number(len)).repeat(Number(rep)).length;
        i += Number(len);
    } else {
        count++;
        i++;
    }
}

console.log(count);
