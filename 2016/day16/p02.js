#!/usr/bin/env node
"use strict";
const fs = require("fs");

// Psuedo random data generator.
function prg(seed, target_len) {
    let a = seed.split("");
    let b = seed.split("").reverse().map(n => Number(n) ^ 1);
    a = a.concat(0, b).join("");
    if( a.length >= target_len ) {
        return a.slice(0, target_len);
    } else {
        return prg(a, target_len);
    }
}

function checksum(data) {
    let a = Array.from(data, Number);
    let partitions = [];
    for(let i = 0; i < data.length; i+=2) {
        partitions.push(a.slice(i, i+2));
    }
    let sum = partitions.map(s => s[0] === s[1] ? 1 : 0).join("");
    if(sum.length % 2) {
        return sum;
    } else {
        return checksum(sum);
    }
}

console.log(checksum(prg(fs.readFileSync(0, "ascii").trim(), 35651584)));
