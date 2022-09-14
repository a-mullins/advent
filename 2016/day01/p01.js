#!/usr/bin/env node
var fs = require("fs");
var input = fs.readFileSync(0, 'ascii'); // 0 is stdin.
input = input.split(', ');

var facing = 0; // 0 = N, 1 = E, 2 = S, 3 = W;
function turnL() { if( --facing < 0 ) { facing = 3 } }
function turnR() { if( ++facing > 3 ) { facing = 0 } }

var x = 0, y = 0;

input.forEach( elem => {
    if( elem[0] == "L" ) { turnL(); }
    if( elem[0] == "R" ) { turnR(); }

    let dist = Number(elem.substr(1));
    
    switch( facing ) {
    case 0:
        y += dist; break;
    case 1:
        x += dist; break;
    case 2:
        y -= dist; break;
    case 3:
        x -= dist; break;
    }
});

console.log( Math.abs(x) +  Math.abs(y) );
