#!/usr/bin/env node
import fs from "node:fs";
import process from "node:process";

import { opt, run } from "./common.mjs";


if(process.argv.includes("-D")) {var DEBUG = true;}
const text = opt(fs.readFileSync(0, "ascii"))
      .split("\n")
      .filter(line => line.length > 0)
      .map(line => line.split(" "));

// I am not proud of this. lol.
let i = 0;
while(true) {
    let looking_for = 0;
    let g = run(text, {"a": i}, DEBUG);
    //console.log(`trying i = ${i}`);
    
    let still_scanning = true;
    let matches = 0;
    let so_far = [];
    while(still_scanning && matches <= 64) {
        let output = g.next().value;
        so_far.push(output);
        //console.log(so_far.join(""));
        if(output === looking_for) {
            //console.log("output DOES match looking_for");
            matches++;
            looking_for ^= 1;
        } else {
            //console.log("output doesn't match looking_for");
            still_scanning = false;
        }
    }
    if(matches > 64) {
        //console.log("got four matches.");
        console.log(i);
        break;
    }
    i++;
}


console.log(`i: ${i}`);
// console.log(
//     ["a", "b", "c", "d"].map(label => `r${label}: ${r[label]}`).join(", "));
