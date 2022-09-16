#!/usr/bin/env node
"use strict";

var fs = require("fs");
var input = fs.readFileSync(0, 'ascii'); // 0 is stdin.
input = input.split('\n').filter( line => line != '' );

var cur = 5;
var left   = [1, 4, 7];
var right  = [3, 6, 9];
var top    = [1, 2, 3];
var bottom = [7, 8, 9];

var code = "";

for( let line of input ) {
    for( let ch of line ) {
        if(ch == "L") { if( !left.includes(cur) ) { cur--; } }
        if(ch == "R") { if( !right.includes(cur) ) { cur++; } }
        if(ch == "U") { if( !top.includes(cur) ) { cur -= 3; } }
        if(ch == "D") { if( !bottom.includes(cur) ) { cur += 3; } }
    }
    code += String(cur);
}

console.log(code);
