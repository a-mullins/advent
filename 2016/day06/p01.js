#!/usr/bin/env node
"use strict";
const fs = require("fs");


let input = fs.readFileSync(0, 'ascii').split('\n');
let codes = input.map( line => line.split('') ).filter(line => line.length > 0);
let counts = Array.from({length: codes[0].length}, () => new Map());
let max_message = "";
let min_message = "";

for(let row = 0; row < codes.length; row++) {
    for(let col = 0; col < counts.length; col++) {
        let letter = codes[row][col];
        counts[col].set( letter, counts[col].get(letter)+1 || 1 );
    }
}

for(const col of counts) {
    let max_count = [...col.values()].reduce((prev, cur) =>
        cur > prev ? cur : prev);
    let min_count = [...col.values()].reduce((prev, cur) =>
        cur < prev ? cur : prev);
    max_message += [...col].find( elem => elem[1] == max_count )[0];
    min_message += [...col].find( elem => elem[1] == min_count )[0];
}

console.log("Max-count Message: %s\nMin-count Message: %s",
            max_message, min_message);
