#!/usr/bin/env node
"use strict";
import fs from "node:fs";


class Display {
    constructor(width, height) {
        this.grid = Array.from({length: height}, () =>
            new Array(width).fill(" "));
    }

    toString() {
        return this.grid.reduce((prev, cur) => prev+cur.join("")+"\n", "");
    }

    rect(width, height) {
        for(let row=0; row<height; row++) {
            for(let col=0; col<width; col++) {
                this.grid[row][col] = "#";
            }
        }
    }

    rot_col(col, count) {
        let old_col;
        for(let cur=0; cur < count; cur++) {
            old_col = this.grid.map(row => row[col]);
            for(let row=0; row<this.grid.length; row++) {
                this.grid[row][col] = old_col.at(row-1);
            }
        }
    }

    rot_row(row, count) {
        let old_row;
        for(let cur=0; cur < count; cur++) {
            old_row = Array.from(this.grid[row]);
            for(let i=0; i<this.grid[row].length; i++) {
                this.grid[row][i] = old_row.at(i-1);
            }
        }
    }
}


const disp = new Display(50, 6);
const lines = fs.readFileSync(0, "ascii").split("\n").filter(l => l != "");

for(const line of lines) {
    if(line.includes("rect")) {
        const [w, h] = line.match(/(\d+)x(\d+)/).slice(1,3).map(Number);
        disp.rect(w, h);
    }
    if(line.includes("rotate col")) {
        const [col, amt] = line.
              match(/x=(\d+) by (\d+)/).
              slice(1,3).
              map(Number);
        disp.rot_col(col, amt);
    }
    if(line.includes("rotate row")) {
        const [row, amt] = line.
              match(/y=(\d+) by (\d+)/).
              slice(1,3).
              map(Number);
        disp.rot_row(row, amt);
    }
}

console.log(disp.toString());
console.log(disp.grid.
            flat().
            reduce((sum, c) => sum + (c === "#" ? 1 : 0), 0));
