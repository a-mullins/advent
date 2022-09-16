#!/usr/bin/env node
"use strict";

var fs = require("fs");
var input = fs.readFileSync(0, 'ascii'); // 0 is stdin.

console.log(
    input.split('\n')
        .map(line => {
            return [Number(line.slice( 2,  5)),
                    Number(line.slice( 7, 10)),
                    Number(line.slice(12, 15))].sort( (a, b) => a - b);})
        .filter(sides => sides[0] + sides[1] > sides[2] )
        .length
);
