#!/usr/bin/env node
"use strict";
const fs = require("fs");
const process = require("process");


// ops:
//   1. swpi x y -- swap letters @ indicies
//   2. swpl x y -- swap letters
//   3. rotl x
//   4. rotr x
//   5. rotp x   -- rotatet based on position of letter indicated
//   6. revr x y -- reverse string from index x to y, inclusive
//   7. movi x y -- move letter at index x to index y.


// preprocess :: "" -> [ {opc: "", op0: (int,""), op1: (int,"")}, {..}, .. ]
//      instr :: { opc: "", op0: int or "", op1: int or "" }
function preprocess(text) {
    let output = [];
    const input = text.trim().split("\n");
    for(const line of input) {
        const parts = line.split(" ");
        if(line.includes("swap position")) {
            output.push({opc: "swpi",
                           op0: Number(parts[2]), op1: Number(parts[5])});
        }
        if(line.includes("swap letter")) {
            output.push({opc: "swpl",
                           op0: parts[2], op1: parts[5]});
        }
        if(line.includes("rotate left")) {
            output.push({opc: "rotl",
                           op0: Number(parts[2]), op1: null});
        }
        if(line.includes("rotate right")) {
            output.push({opc: "rotr",
                           op0: Number(parts[2]), op1: null});
        }
        if(line.includes("rotate based on")) {
            output.push({opc: "rotp",
                           op0: parts[6], op1: null});
        }
        if(line.includes("reverse positions")) {
            output.push({opc: "revr",
                           op0: Number(parts[2]), op1: Number(parts[4])});
        }
        if(line.includes("move position")) {
            output.push({opc: "movi",
                           op0: Number(parts[2]), op1: Number(parts[5])});
        }
    }
    return output;
}


let output = (process.argv.includes("--test") ? "decab" : "fbgdceah").split("");
let text = preprocess(fs.readFileSync(0, "ascii")).reverse();


for(let ip = 0; ip < text.length; ip++) {
    let instr = text[ip];
    let tmp, pos0, pos1;

    switch (instr.opc) {
    case "swpi":
        // Involutory.
        tmp = output[instr.op0];

        output[instr.op0] = output[instr.op1];
        output[instr.op1] = tmp;
        break;

    case "swpl":
        // Involutory.
        pos0 = output.indexOf(instr.op0);
        pos1 = output.indexOf(instr.op1);
        tmp = output[pos0];

        output[pos0] = output[pos1];
        output[pos1] = tmp;
        break;

    case "rotl":
        // NOT involutory, but reversible.
        // So actually do rotr.
        tmp = Array.from(output);
        output.length = 0; // clear array.

        for(let i = 0; i < tmp.length; i++) {
            output[i] = tmp.at(i - instr.op0);
        }
        break;

    case "rotr":
        // NOT involutory, but reversible.
        // So actually do a rotl.
        tmp = Array.from(output);
        output.length = 0; // clear array.

        for(let i = 0; i < tmp.length; i++) {
            output[i] = tmp[(i + instr.op0) % tmp.length];
        }
        break;

    case "rotp":
        // NOT involutory, but reversible.
        // We need to figure out where the letter WAS based on where it is
        // now, and then lshift that many times as per the formula.
        // | start@ | rshift by |  ends@ |          | start@ | lshift |
        // |      0 |         1 |      1 |          |      0 |      1 |
        // |      1 |         2 |      3 |          |      1 |      1 |
        // |      2 |         3 |      5 |  invert  |      2 |      6 |
        // |      3 |         4 |      7 | -------> |      3 |      2 |
        // |      4 |         6 | 10%8=28|          |      4 |      7 |
        // |      5 |         7 | 12%8=4 |          |      5 |      3 |
        // |      6 |         8 | 14%8=6 |          |      6 |      0 |
        // |      7 |         9 | 16%8=0 |          |      7 |      4 |
        const rotMap = [1, 1, 6, 2, 7, 3, 0, 4];
        
        tmp = Array.from(output);
        output.length = 0; // clear array.
        pos0 = tmp.indexOf(instr.op0);
        
        // How many times do we need to lshift?
        instr.op1 = rotMap[pos0];

        for(let i = 0; i < tmp.length; i++) {
            output[i] = tmp[(i + instr.op1) % tmp.length];
        }
        break;

    case "revr":
        // Involutory.
        tmp = Array.from(output);
        output.length = 0; // clear array.
        // in place, alloc one less Array?
        output.splice(0, Infinity,
                      ...tmp.slice(0, instr.op0),
                      ...tmp.slice(instr.op0, instr.op1+1).reverse(),
                      ...tmp.slice(instr.op1 + 1));
        break;

    case "movi":
        // NOT involutory, but reversible.
        // We can just swap the args, so that pos y gets put at pos x.
        tmp = output.splice(instr.op1, 1)[0];
        output.splice(instr.op0, 0, tmp);
        break;

    default:
        fs.writeSync(2, "ERR: Unrecognized opcode. Quitting.\n");
        process.exit(1);
    }
}

console.log(output.join(""));
