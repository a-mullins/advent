#!/usr/bin/env node
"use strict";
const fs = require("fs");
const process = require("process");


function nextRow(row) {
    const len = row.length;
    const oldRow = row + ".";
    const newRow = [];

    for(let i = 0; i < len; i++) {
        let section = oldRow.at(i-1) + oldRow.slice(i,i+2);
        if(section === "^^." || section === ".^^" ||
           section === "^.." || section === "..^")
        {
            newRow.push("^");
        } else {
            newRow.push(".");
        }
    }

    return newRow.join("");
}


function numSafes(row) {
    let n = 0;
    for(const c of row) {
        if(c === ".") {
            n++;
        }
    }
    return n;
}


let count = 0;
{
    const maxRows = process.argv[1].includes("p02") ? 400000 : 40;
    let row = fs.readFileSync(0, "ascii").trim();
    let i = 0;
    do {
        count += numSafes(row);
        row = nextRow(row);
        i++;
    } while(i < maxRows);
}
console.log(count);
