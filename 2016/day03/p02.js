#!/usr/bin/env node
"use strict";

var fs = require("fs");
var input = fs.readFileSync(0, 'ascii'); // 0 is stdin.
input = input.split('\n').filter(line => line != '').map(line => {
        return [Number(line.slice( 2,  5)),
                Number(line.slice( 7, 10)),
                Number(line.slice(12, 15))];});

var tris = [];
for(let row = 0; row < input.length; row += 3) {
    for(let col = 0; col < 3; col++) {
        let sides = [input[row  ][col],
                     input[row+1][col],
                     input[row+2][col]].sort( (a, b) => a-b );
        if( sides[0] + sides[1] > sides[2] ) {
            tris.push(sides);
        }
    }
}

console.log(tris.length);
