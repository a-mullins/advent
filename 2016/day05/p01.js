#!/usr/bin/env node
"use strict";
const fs = require("fs");
const crypto = require("crypto");


const input = fs.readFileSync(0, "ascii").trim();
let password = "";
let index = 0;
while(password.length < 8) {
    let next = crypto.createHash("md5")
        .update(input + String(index)).digest("hex");
    if(next.slice(0, 5) === "00000") {
        password += next[5];
        console.log(input+String(index), next, password);
    }
    index++;
}
