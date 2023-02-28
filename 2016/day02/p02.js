#!/usr/bin/env node
"use strict";
const fs = require("fs");


const keypad = ["       ",
                "   1   ",
                "  234  ",
                " 56789 ",
                "  ABC  ",
                "   D   ",
                "       "];
let row = 3;
let col = 1;
let code = "";

const input = fs
      .readFileSync(0, 'ascii').split('\n').filter( line => line != '' );

for(const line of input) {
    for(const c of line) {
        let tmp_row = row,
            tmp_col = col;
        if(c === "L") {--tmp_col;}
        if(c === "R") {++tmp_col;}
        if(c === "U") {--tmp_row;}
        if(c === "D") {++tmp_row;}
        if(keypad[tmp_row][tmp_col] !== ' ') {
            row = tmp_row;
            col = tmp_col;
        }
    }
    code += keypad[row][col];
}

console.log(code);
