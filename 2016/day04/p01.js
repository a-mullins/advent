#!/usr/bin/env node
"use strict";

var fs = require("fs");
var input = fs.readFileSync(0, 'ascii'); // 0 is stdin.
input = input.split('\n').filter( line => line != '' );

var sum = 0;

for( let line of input ) {
    let map = {};
    let their_checksum = line.slice(line.indexOf('[')+1, line.indexOf(']'));
    let checksum = "";
    let sector = Number(/\d+/.exec(line));
    for( let ch of line ) {
        if(ch == '[') { break; }
        if(/[a-z]/.test(ch)) { map[ch] = map[ch] + 1 || 1; }
    }
    let letter_counts = new Set(Object.values(map).sort((a, b)=>b-a));
    for( let count of letter_counts ) {
        checksum += Object.keys(map)
            .filter( key => map[key] == count)
            .sort().join('');
    }
    checksum = checksum.slice(0, 5);
    if(their_checksum == checksum) {
        sum += sector;
    }
}

console.log(sum);
