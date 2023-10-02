#!/usr/bin/env node
"use strict";
import fs from "node:fs";


function shiftStr(str, amount) {
    let output = "";
    for(let i = 0; i < str.length; i++) {
        let c = (str.charCodeAt(i)-97+amount) % 26 + 97;
        output += String.fromCharCode(c);
    }
    return output;
}


const input = fs.readFileSync(0, "ascii")
      .trim()
      .split("\n");
for(const line of input) {
    const sector = Number(/\d+/.exec(line));
    let words = line.split("-");
    words.pop(); // don't need the last elem.

    words = words.map(word => shiftStr(word, sector)).join(" ");
    if(words.includes("north")) {
        console.log(line, words, sector);
    }
}
