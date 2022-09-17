#!/usr/bin/env node
"use strict";
const fs = require('fs');

function splitAddy(s) {
    let parts = {normal: [], hypernet: []}
    let start = 0;
    for(let i = 0; i < s.length; i++) {
        if(i+1 == s.length && s[i] != ']') {
            parts.normal.push(s.slice(start, i+1));
        }
        if(s[i] == '[') {
            parts.normal.push(s.slice(start, i));
            start = i+1;
        }
        if(s[i] == ']') {
            parts.hypernet.push(s.slice(start, i));
            start = i+1;
        }
    }
    return parts;
}

function hasAbba(s) {
    function _isAbba(s) {
        return s[0] != s[1] && s[0] == s[3] && s[1] == s[2];
    }
    
    let found = false;
    for(let i = 0; i+4 <= s.length; i++) {
        found = found || _isAbba(s.slice(i, i+4));
    }
    return found;
}


let lines = fs.readFileSync(0, 'ascii').split('\n').filter(s => s.length > 0);
//let lines = fs.readFileSync('/Users/alm/Code/advent/2016/day07/test', 'ascii').split('\n').filter(s => s.length > 0);
let addys = lines.map( splitAddy );

let count = 0;
for( let addy of addys ) {
    if( addy.hypernet.filter( hasAbba ).length > 0 ) {
        continue;
    }
    if( addy.normal.filter( hasAbba ).length > 0 ) {
        count++;
    }
}

console.log(count);
