#!/usr/bin/env node
const fs = require("fs");

if(process.argv.includes("-D")) {var DEBUG = true;}

r = {"a": 0, "b": 0, "c": 0, "d": 0};
text = fs.readFileSync(0, "ascii")
    .split("\n")
    .map(line => line.split(" "));

// ip = instruction pointer, program counter, &c.
for(let ip = 0; 0 <= ip && ip < text.length; /*nop*/) {
    instr = text[ip];
    if(DEBUG) {
        console.log(`ip = ${ip}; instr = ${instr.join(" ")};\n`
                    + ["a", "b", "c", "d"]
                    .map(label => `r${label}: ${r[label]}`).join(', '));
    }
    switch (instr[0]) {
    case "cpy":
        let literal = Number(instr[1]); // could be NaN
        if(literal) {
            r[instr[2]] = literal;
            if(DEBUG) {
                console.log(`\tcpy lit ${literal} → ${instr[2]}`);
            }
        } else {
            r[instr[2]] = r[instr[1]];
            if(DEBUG) {
                console.log(`\tcpy ${instr[1]} (${r[instr[1]]}) → ${instr[2]}`);
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
        if(r[instr[1]] !== 0) {
            ip += Number(instr[2]);
            if(DEBUG){
                console.log(`\tjnz ${instr[1]} = ${r[instr[1]]}; now ip = ${ip}`);
            }
        } else {
            ip++;
            if(DEBUG) {
                console.log(`\tjnz ${instr[1]} = `
                            + `${r[instr[1]]}; no jump, ip advanced`);
            }
        }
        break;

    default:
        ip++;
    }
}

if(DEBUG) {console.log("");}
console.log("Program terminated.\n\t" + 
    ["a", "b", "c", "d"].map(label => `r${label}: ${r[label]}`).join(', '));
