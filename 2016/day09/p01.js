#!/usr/bin/env node
"use strict";


const fs = require('fs');
const input = fs.readFileSync(0, 'ascii').trim();

var output = "";

for(let i = 0; i < input.length; ) {
    if( input[i] == '(' ) {
        let re = RegExp(/(\d+)x(\d+)/);
        let [ m, len, rep] = re.exec(input.slice(i+1));
        i += m.length + 2;

        output += input.slice(i, i+Number(len)).repeat(Number(rep));
        i += Number(len);
    } else {
        output += input[i];
        i++
    }
}

console.log(output.length);
