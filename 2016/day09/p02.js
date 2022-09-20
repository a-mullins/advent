#!/usr/bin/env node
"use strict";


const fs = require('fs');


function decom(s) {
    let m = /\((\d+)x(\d+)\)/.exec(s);
    if( !m ) { // no match
        return s;
    }
    //console.log(m);
    let [text, run, rep] = m;
    let i = m.index;
    return s.slice(0, i) +
        decom(s.slice(i+text.length, i+text.length + Number(run)).repeat(rep)) +
        decom(s.slice(i+text.length + Number(run)));
}

function decom_len(s) {
    let m = /\((\d+)x(\d+)\)/.exec(s);
    if( !m ) { // no match
        return s.length;
    }
    //console.log(m);
    let [text, run, rep] = m;
    let i = m.index;
    return Number(s.slice(0, i).length) +
        decom_len(s.slice(i+text.length, i+text.length + Number(run)).repeat(rep)) +
        decom_len(s.slice(i+text.length + Number(run)));
}


var input = fs.readFileSync(0, 'ascii').trim();
console.log(decom_len(input));
