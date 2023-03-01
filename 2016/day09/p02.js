#!/usr/bin/env node
// TODO multiprocess/threading candidate?
"use strict";
const fs = require("fs");


function decomp_len(s) {
    let m = /\((\d+)x(\d+)\)/.exec(s);
    if( !m ) { // no match
        return s.length;
    }
    let [text, run, rep] = m;
    let i = m.index;
    return Number(s.slice(0, i).length)
        + decomp_len(s.slice(i+text.length, i+text.length+Number(run)).repeat(rep))
        + decomp_len(s.slice(i+text.length + Number(run)));
}


let input = fs.readFileSync(0, 'ascii').trim();
console.log(decomp_len(input));
