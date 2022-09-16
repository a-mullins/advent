#!/usr/bin/env node
"use strict";
var fs = require("fs");
var crypto = require("crypto");

var input = fs.readFileSync(0, 'ascii').trim(); // 0 is stdin.
var password = "";
var index = 0;

while( password.length < 8 ) {
    let next = crypto.createHash('md5').update(input + String(index))
                     .digest('hex');
    if(next.slice(0, 5) == '00000') {
        password += next[5];
        console.log(input+String(index), next, password);
    }
    index++;
}
