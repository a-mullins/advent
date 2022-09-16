#!/usr/bin/env node
"use strict";
var fs = require("fs");
var crypto = require("crypto");

var input = fs.readFileSync(0, 'ascii').trim(); // 0 is stdin.
var password = Array(8).fill(null);
var num_found = 0;
var index = 0;

while( num_found < 8 ) {
    let next = crypto.createHash('md5').update(input + String(index))
                     .digest('hex');
    if( next.slice(0, 5) == '00000' && Number(next[5]) < 8
        && !password[next[5]] ) {
        password[ next[5] ] = next[6];
        console.log(input+String(index), next,
                    password.map( c => c ? c : '_' ).join(''));
        num_found++;
    }
    index++;
}
