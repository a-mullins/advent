#!/usr/bin/env node
"use strict";

var fs = require("fs");


var DEBUG = process.argv.includes('-d');


// lines have the form of either:
// value N goes to bot B
// bot B gives low to (bot|output) L and high to (bot|output) H
var lines  = fs.readFileSync(0, 'ascii').trim().split('\n');
var rules  = lines.filter(line => line.includes('gives'));
rules = rules.map(rule => rule.split(' '));
var inputs = lines.filter(line => line.includes('goes'));
inputs = inputs.map(inp => inp.split(' '));
var bots = [];
var bins = [];

for(let parts of rules) {
    bots.push({num: Number(parts[1]),
               low_target: Number(parts[6]),
               low_type: parts[5],
               high_target: Number(parts[11]),
               high_type: parts[10],
               chips: []});
    if(parts[5] == 'output') {
        bins.push({num: Number(parts[6]),
                   chips: []});
    }
    if(parts[10] == 'output') {
        bins.push({num: Number(parts[11]),
                   chips: []});
    }
}

for( let parts of inputs ) {
    let bot  = bots.find( bot => bot.num == Number(parts[5]));
    if(bot.chips.length < 2) {
        if(DEBUG) console.log("chip %d => bot %d",
                              Number(parts[1]), bot.num);
        bot.chips.push(Number(parts[1]));
    } else {
        if(DEBUG) console.log("bot %d full, can't add chip %s",
                              bot.num, parts[1]);
        inputs.push(parts);
    }

    let candidates = [bot];
    for( let cur of candidates ) {
        if(cur.chips.length >= 2) {
            let [low, high] = cur.chips.sort( (a,b) => a-b );
            cur.chips = [];
            if(low == 17 && high == 61) {
                console.log("bot %d handled 17 and 61", cur.num);
            }
            if(DEBUG) {console.log("bot %d has chips %d, %d", cur.num,
                                   low, high);}

            if(cur.low_type == 'output') {
                let bin = bins.find( bin => bin.num == cur.low_target );
                bin.chips.push(low);
                if(DEBUG) {console.log("giving chip %d to bin %d",
                                       low, cur.low_target);}
            }
            if(cur.high_type == 'output') {
                let bin = bins.find( bin => bin.num == cur.high_target );
                bin.chips.push(high);
                if(DEBUG) {console.log("giving chip %d to bin %d",
                                       high, cur.high_target);}
            }
            if(cur.low_type == 'bot') {
                let target = bots.find( b => b.num == cur.low_target );
                target.chips.push(low);
                if(DEBUG) {console.log("giving chip %d to bot %d",
                                      low, target.num);}
                if(target.chips.length >= 2) {
                    candidates.push(target);
                }
            }
            if(cur.high_type == 'bot') {
                let target = bots.find( b => b.num == cur.high_target );
                target.chips.push(high);
                if(DEBUG) {console.log("giving chip %d to bot %d",
                                       high, target.num);}
                if(target.chips.length >= 2) {
                    candidates.push(target);
                }
            }
        }
    }
}

var output = bins.filter(bin => [0, 1, 2].includes(bin.num))
output = output.map(bin => bin.chips[0]).reduce((prev,cur) => prev*cur, 1);
console.log("multiplied vals of 0, 1, and 2: %d", output);
