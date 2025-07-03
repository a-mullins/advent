#!/usr/bin/env node
// Copyright (c) 2025 Adam Mullins
//
// Solution for Advent of Code 2016, Day 14, Part 1
//   https://adventofcode.com/2016/day/14
"use strict";
import fs from "node:fs";
import crypto from "node:crypto";


function findTriple(s) {
    let m = s.match(/(.)\1{2}/);
    return m ? m[1] : null;
}


function hasQuint(s, c)  {return s.includes(c.repeat(5));}


function hash(idx) {
    let s = crypto.createHash("md5").update(salt + String(idx)).digest("hex");
    if(process.argv[1].includes("p02")) {
        for(let i = 0; i < 2016; i++) {
            s = crypto.createHash("md5").update(s).digest("hex");
        }
    }
    return s;
}


const salt = fs.readFileSync(0, "ascii").trim("\n");
//const salt = "abc";
const hashes = [];
const keys = [];

let i = 0;
while(keys.length < 64) {
    let isKey = false;
    // iterate forward until triple.
    if(!hashes[i]) {hashes[i] = hash(i);}
    const trip_c = findTriple(hashes[i]);
    if(trip_c) {
        //console.log(`found trip @ i${i}, c=${trip_c} looking for quint`);
        // if there is a quintuple in the next 1000 hashes, hash[i] is a key.
        for(let j = i+1; j < (i+1000); j++) {
            //fs.writeFileSync(1, `${j} < ${i+1000}`);
            if(!hashes[j]) {hashes[j] = hash(j);}
            //console.log(`j = ${j}; checking hashes[j] = ${hashes[j]}`);
            if(hasQuint(hashes[j], trip_c)) {
                //console.log(`found quint @ hashes[${j}]=${hashes[j]}`);
                isKey = true;
                break;
            }
        }
    }
    if(isKey) {keys.push({index: i, hash: hashes[i]});}
    // -[ DEBUG ]-----------------------------------------------------
    // const n = i<10 ? " " + String(i) : String(i);
    // console.log(`${n}: ${hashes[i]} `+
    //             `${hasTriple(hashes[i])?"triple":"      "} `+
    //             `${hasQuint(hashes[i])?"quintuple":"         "}`);
    // -[ DEBUG ]-----------------------------------------------------
    i++;
}
//console.log(hashes);
console.log("Keys:");
for(let i = 0; i<keys.length; i++) {
    const n   = (i+1)<10 ? " " + String(i+1) : String(i+1);
    console.log(`${n}, ${keys[i].index}: ${keys[i].hash} `);
}
//console.log(keys);
