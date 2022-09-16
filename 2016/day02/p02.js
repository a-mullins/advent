#!/usr/bin/env node
"use strict";

var keypad =
    [ "       ",
      "   1   ",
      "  234  ",
      " 56789 ",
      "  ABC  ",
      "   D   ",
      "       "];
var row = 3, col = 1;
var code = ""

var fs = require("fs");
var input = fs.readFileSync(0, 'ascii'); // 0 is stdin.
input = input.split('\n').filter( line => line != '' );

for( let line of input ) {
    for( let ch of line ) {
        let tmp_row = row, tmp_col = col;
        if(ch == "L") {--tmp_col;}
        if(ch == "R") {++tmp_col;}
        if(ch == "U") {--tmp_row;}
        if(ch == "D") {++tmp_row;}
        if( keypad[tmp_row][tmp_col] && keypad[tmp_row][tmp_col] != ' ' ) {
            row = tmp_row;
            col = tmp_col;
        }
    }
    code += keypad[row][col];
}

console.log(code);
