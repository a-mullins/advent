#!/usr/bin/env node
"use strict";

function shiftStr(str, amount) {
    let output = ""
    for( let i = 0; i < str.length; i++ ) {
        let chr = (str.charCodeAt(i)-97+amount) % 26 + 97;
        output += String.fromCharCode( chr );
    }
    return output
}

var fs = require("fs");
var input = fs.readFileSync(0, 'ascii'); // 0 is stdin.
input = input.split('\n').filter( line => line != '' );

var sum = 0;

for( let line of input ) {
    let map = {};
    let sector = Number(/\d+/.exec(line));
    let words = line.split('-');
    words.pop(); // don't need the last elem.

    words = words.map( word => shiftStr(word, sector) ).join(' ');
    if( words.includes('north') )
        console.log(line, words, sector);
}
