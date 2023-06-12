#!/usr/bin/env node
const fs = require("fs");
const process = require("process");


if(process.argv.includes("-D")) {var DEBUG = true;}

let r = {"a": 7, "b": 0, "c": 0, "d": 0};
if(process.argv[1].includes("p02")) {
    if(DEBUG) {console.log("Called as p02");}
    r["a"] = 12;
}

const text = fs.readFileSync(0, "ascii")
      .split("\n")
      .filter(line => line.length > 0)
      .map(line => line.split(" "));

// ip = instruction pointer, program counter, &c.
for(let ip = 0; 0 <= ip && ip < text.length; /*nop*/) {
    let instr = text[ip];
    if(DEBUG) {
        console.log(`ip = ${ip}; instr = ${instr.join(" ")};\n`
                    + ["a", "b", "c", "d"]
                    .map(label => `r${label}: ${r[label]}`).join(', '));
    }

    switch (instr[0]) {
    case "cpy":
        // second arg must be register name. if it's not, nop.
        if(!(instr[2] in r)) {
            if(DEBUG) { console.log(`\tbad cpy dest, skipping.`); }
            ip++;
            break;
        }

        if(isNaN(instr[1])) {
            r[instr[2]] = r[instr[1]];
            if(DEBUG) {
                console.log(`\tcpy reg ${instr[1]}`
                            + `(${r[instr[1]]}) → ${instr[2]}`);
            }
        } else {
            r[instr[2]] = Number(instr[1]);
            if(DEBUG) {
                console.log(`\tcpy lit ${instr[1]} → ${instr[2]}`);
            }
        }
        ip++;
        break;

    case "inc":
        r[instr[1]]++;
        if(DEBUG) {
            console.log(`\tinc ${instr[1]}, now ${r[instr[1]]}`);
        }
        ip++;
        break;

    case "dec":
        r[instr[1]]--;
        if(DEBUG) {
            console.log(`\tdec ${instr[1]}, now ${r[instr[1]]}`);
        }
        ip++;
        break;

    case "jnz":
        // jnz x y
        // x must be register label, y must be integer.
        let val = instr[1] in r
            ? r[instr[1]]
            : Number(instr[1]);

        let dist = instr[2] in r
            ? r[instr[2]]
            : Number(instr[2]);

        if(val !== 0) {
            ip += dist;
            if(DEBUG){
                console.log(`\tjnz ${val} ${dist}; now ip = ${ip}`);
            }
        } else {
            ip++;
            if(DEBUG) {
                console.log(`\tjnz ${val} ${dist}; no jump, ip advanced`);
            }
        }
        break;

    case "tgl":
        let i = ip + Number(r[instr[1]]);
        let target = text[ip + Number(r[instr[1]])];

        if(DEBUG) {
            console.log(`\ttgl ${instr[1]},`
                        + ` r${instr[1]} = ${r[instr[1]]}; `);
        }
        if(i < 0 || i >= text.length) {
            if(DEBUG) { console.log('\ttgl index out of range, nop'); }
            ip++;
            break;
        }
        if(DEBUG) {
            console.log(`\ttarget: text[${i}] = ${target.join(" ")}`);
        }

        // 1-arg
        if(target.length === 2) {
            if(target[0]=== "inc") {
                target[0] = "dec";
            } else {
                target[0] = "inc";
            }
        }
        // 2-arg
        if(target.length === 3) {
            if(target[0] === "jnz") {
                target[0] = "cpy";
            } else {
                target[0] = "jnz";
            }
        }

        if(DEBUG) {
            console.log(`\ttarget: text[${i}] = ${target.join(" ")}`);
        }
        ip++;
        break;

    default:
        fs.writeSync(2, "ERR: Unrecognized opcode. Quitting.\n");
        process.exit(1);
    }
    if(DEBUG) {console.log("");}
}

if(DEBUG) {console.log("");}
console.log("Program halts.\n\t"
            + ["a", "b", "c", "d"]
            .map(label => `r${label}: ${r[label]}`).join(', '));
