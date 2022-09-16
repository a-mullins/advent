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
        if(ch == "L") {if( keypad[tmp_row][--tmp_col] != ' ' )
                       {row = tmp_row; col = tmp_col;}}
        if(ch == "R") {if( keypad[tmp_row][++tmp_col] != ' ' )
                       {row = tmp_row; col = tmp_col;}}
        if(ch == "U") {if( keypad[--tmp_row][tmp_col] != ' ' )
                       {row = tmp_row; col = tmp_col;}}
        if(ch == "D") {if( keypad[++tmp_row][tmp_col] != ' ' )
                       {row = tmp_row; col = tmp_col;}}
    }
    code += keypad[row][col];
}

console.log(code);
