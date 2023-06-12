#!/usr/bin/env node
"use strict";
const fs = require("fs");


// lines have the form of either:
// value N goes to bot B
// bot B gives low to (bot|output) L and high to (bot|output) H
const lines  = fs.readFileSync(0, "ascii").trim().split("\n");
const rules  = lines
      .filter(line => line.includes("gives"))
      .map(rule => rule.split(" "));
const inputs = lines
      .filter(line => line.includes("goes"))
      .map(inp => inp.split(" "));
const bots = [];
const bins = [];

for(const parts of rules) {
    bots.push({num: Number(parts[1]),
               low_target: Number(parts[6]),
               low_type: parts[5],
               high_target: Number(parts[11]),
               high_type: parts[10],
               chips: []});
    if(parts[5] == "output") {
        bins.push({num: Number(parts[6]),
                   chips: []});
    }
    if(parts[10] == "output") {
        bins.push({num: Number(parts[11]),
                   chips: []});
    }
}

for(const parts of inputs) {
    const bot = bots.find((bot) => bot.num == Number(parts[5]));
    if(bot.chips.length < 2) {
        bot.chips.push(Number(parts[1]));
    } else {
        inputs.push(parts);
    }

    let candidates = [bot];
    for(const cur of candidates) {
        if(cur.chips.length >= 2) {
            const [low, high] = cur.chips.sort((a,b) => a-b);
            cur.chips = [];
            if(low == 17 && high == 61) {
                console.log(`bot ${cur.num} handled 17 and 61`);
            }
            if(cur.low_type == "output") {
                const bin = bins.find( bin => bin.num == cur.low_target );
                bin.chips.push(low);
            }
            if(cur.high_type == "output") {
                const bin = bins.find( bin => bin.num == cur.high_target );
                bin.chips.push(high);
            }
            if(cur.low_type == "bot") {
                const target = bots.find( b => b.num == cur.low_target );
                target.chips.push(low);
                if(target.chips.length >= 2) {
                    candidates.push(target);
                }
            }
            if(cur.high_type == "bot") {
                const target = bots.find( b => b.num == cur.high_target );
                target.chips.push(high);
                if(target.chips.length >= 2) {
                    candidates.push(target);
                }
            }
        }
    }
}

const output = bins
      .filter(bin => 0 <= bin.num && bin.num <= 2)
      .map(bin => bin.chips[0])
      .reduce((prev,cur) => prev*cur, 1);
console.log(`multiplied vals of 0, 1, and 2: ${output}`);
