#!/usr/bin/env node
"use strict";


function opt(text) {
    // addAssign has the pattern:
    //   inc a
    //   dec c
    //   jnz c -2
    // and condenses to a += c; c = 0;
    // If we introduce and add x y that adds the value of x to y, we can
    // apply a peephole optimization.
    //   add x y -> y += x;
    const addAssign = /inc ([abcd])\ndec ([abcd])\njnz \2 -2/g;
    text = text.replaceAll(addAssign, "add $2 $1\ncpy 0 $2\njnz 0 0");
    return text;
}


function run(text, registers, DEBUG) {
    const r = {"a": 0, "b": 0, "c": 0, "d": 0};
    if(typeof registers !== "undefined") {
        for(const val in r) {
            r[val] = registers[val] || r[val];
        }
    }
    // ip = instruction pointer, program counter, &c.
    for(let ip = 0; 0 <= ip && ip < text.length; /*nop*/) {
        const instr = text[ip];
        if(DEBUG) {
            console.log(`ip = ${ip}; instr = ${instr.join(" ")};\n`
                        + ["a", "b", "c", "d"]
                        .map(label => `r${label}: ${r[label]}`).join(", "));
        }

        switch (instr[0]) {
        case "cpy":
            // second arg must be register name. if it's not, nop.
            if(!(instr[2] in r)) {
                if(DEBUG) {console.log(`\tbad cpy dest, skipping.`);}
                ip++;
                break;
            }

            if(isNaN(instr[1])) { // if src is NaN, it's a register label
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
                if(DEBUG) { console.log("\ttgl index out of range, nop"); }
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

        case "add":
            //add x y -> y += x;
            //console.log(`r[instr[0]] -> r[${instr[0]}] -> ${r[instr[0]]}`);
            r[instr[2]] += r[instr[1]];
            ip++;
            break;

        default:
            console.log("ERR: Unrecognized opcode. Quitting.");
            console.log(`ERR: ip: ${ip}; text[${ip}]: ${instr.join(" ")}`);
            process.exit(1);
        }
        if(DEBUG) {console.log("");}
    }
    return r;
}


export { opt, run };
