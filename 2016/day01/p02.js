#!/usr/bin/env node
"use strict";

var fs = require("fs");
var input = fs.readFileSync(0, 'ascii'); // 0 is stdin.
input = input.split(', ');

var facing = 0; // 0 = N, 1 = E, 2 = S, 3 = W;
function turnL() { if( --facing < 0 ) { facing = 3 } }
function turnR() { if( ++facing > 3 ) { facing = 0 } }

var x = 0, y = 0;

var visited = [];

function range(start, end) {
    if( !end ) { end = start, start = 0;}
    if( start == end ) { return [] }
    else { return [start].concat(range(start+1, end)) }
}

var dup = undefined;

for( elem of input ) {
    if( elem[0] == "L" ) { turnL(); }
    if( elem[0] == "R" ) { turnR(); }

    let dist = Number(elem.substr(1));

    for(i of range(dist)) {
        if( facing == 0 ) { y++; }
        if( facing == 1 ) { x++; }
        if( facing == 2 ) { y--; }
        if( facing == 3 ) { x--; }

        cur = String(x) + " " + String(y);
        if(visited.includes(cur)) {
            dup = cur;
            visited.push(cur);
            console.log('dup found: ' + cur +
                        '\ndist: ' + (Math.abs(x) +  Math.abs(y)));
            break;
        } else {
            visited.push(cur);
        }
    }

    if( dup ) { break; }
}
