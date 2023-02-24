#!/usr/bin/env node
"use strict";
const fs = require("fs");


const input = fs.readFileSync(0, 'ascii').split('\n').filter(line => line != '');
let sum = 0;
for(let line of input) {
    const map = {},
          their_checksum = line.slice(line.indexOf('[')+1, line.indexOf(']')),
          sector = Number(/\d+/.exec(line));
    let checksum = "";    
    for(const c of line) {
        if(c === '[') { break; }
        if(/[a-z]/.test(c)) {
            map[c] = map[c] + 1 || 1;
        }
    }
    const letter_counts = new Set(Object.values(map).sort((a, b)=>b-a));
    for(const count of letter_counts) {
        checksum += Object.keys(map)
            .filter(key => map[key] === count)
            .sort().join('');
    }
    checksum = checksum.slice(0, 5);
    if(their_checksum === checksum) {
        sum += sector;
    }
}

console.log(sum);
