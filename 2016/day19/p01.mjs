#!/usr/bin/env node
"use strict";
import fs from "node:fs";


function ring(n) {
    const start = {elf: 0, presents: 1, next: null, prev: null};
    {
        let old = start;
        for(let i = 1; i<n; i++) {
            let cur = {elf: i, presents: 1, next: null, prev: old};
            old.next = cur;
            old = cur;
        }
        old.next = start;
        start.prev = old;
    }
    return start;
}


function del(elf) {
    elf.prev.next = elf.next;
    elf.next.prev = elf.prev;
}


const numElves = Number(fs.readFileSync(0, "ascii").trim());
let cur = ring(numElves);
while(cur.next !== cur) {
    // console.log(`Elf ${cur.elf + 1} has ${cur.presents} presents.\n` +
    //             `\tElf ${cur.next.elf + 1} is to their left.`);
    if(cur.presents === 0) {
        del(cur);
        // console.log("\tNo presents! Leaving the circle.");
    } else {
        cur.presents += cur.next.presents;
        // console.log(`\tElf ${cur.elf + 1} took ${cur.next.presents} presents from them` +
        //             ` and now has ${cur.presents}.`);
        cur.next.presents = 0;
    }

    cur = cur.next;
}

console.log(`Elf ${cur.elf + 1} got all ${cur.presents} presents!`);
