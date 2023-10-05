#!/usr/bin/env node
"use strict";
import fs from "node:fs";
// The "trick" is to print out the grid and solve it by hand.  Move the empty
// grid step by step. Note that there are some "walls", but they can be easily
// identified because they are the nodes that have more than 100 TB of data.


let grid = [];
let max_row = 0;
let max_col = 0;
{ // block so that temp vars go out of scope.
    const text = fs.readFileSync(0, "ascii");
    const lines = text.
          slice(text.match(/\//).index).
          trim().
          split("\n");

    const tmp = [];
    for(const line of lines) {
        const parts = line.replace(/\s+/g, " ").split(" ");

        tmp.push({
            col: Number(parts[0].match(/x(\d+)/)[1]),
            row: Number(parts[0].match(/y(\d+)/)[1]),
            size: Number(parts[1].match(/\d+/g)[0]),
            used: Number(parts[2].match(/\d+/g)[0]),
            goal: false
        });
    }
    max_col = Math.max(...tmp.map(node => node.col));
    max_row = Math.max(...tmp.map(node => node.row));
    grid = Array.from(
        Array(max_row+1),
        () => new Array(max_col+1)
    );
    for(const node of tmp) {
        grid[node.row][node.col] = node;
    }

    grid[0][max_col].goal = true;
}

grid.toString = () => {
    const repr = [];
    for(let row=0; row < max_row+1; row++) {
        for(let col=0; col < max_col+1; col++) {
            if(row === 0 && col === 0) {
                repr.push("(");
            } else {
                repr.push(" ");
            }

            if(grid[row][col].goal)  {
                repr.push("G");
            } else if(grid[row][col].used === 0) {
                repr.push("_");
            } else if(grid[row][col].used > 99) {
                repr.push("#");
            } else {
                repr.push(".");
            }

            if(row === 0 && col === 0) {
                repr.push(")");
            } else {
                repr.push(" ");
            }
        }
        repr.push("\n");
    }
    return repr.join("");
}

console.log(""+grid);
