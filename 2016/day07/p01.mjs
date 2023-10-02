#!/usr/bin/env node
"use strict";
import fs from "node:fs";


function splitAddy(s) {
    const parts = {supernet: [], hypernet: []};
    let start = 0;
    for(let i = 0; i < s.length; i++) {
        if(i+1 === s.length && s[i] != "]") {
            parts.supernet.push(s.slice(start, i+1));
        }
        if(s[i] === "[") {
            parts.supernet.push(s.slice(start, i));
            start = i+1;
        }
        if(s[i] === "]") {
            parts.hypernet.push(s.slice(start, i));
            start = i+1;
        }
    }
    return parts;
}


function isAbba(s) {
    return s[0] != s[1] && s[0] === s[3] && s[1] === s[2];
}


function hasAbba(s) {
    let found = false;
    for(let i = 0; i+4 <= s.length; i++) {
        found ||= isAbba(s.slice(i, i+4));
    }
    return found;
}


function supportsTls(addy) {
    return addy.supernet.some(hasAbba) && !(addy.hypernet.some(hasAbba));
}


function supportsSsl(addy) {
    // regex abuse to find overlapping matches:
    const aba_pattern = /(?=((\w)(?!\2).\2))/gi;
    const aba_matches = [];
    for(let supernet of addy.supernet) {
        let matches = Array.from(supernet.matchAll(aba_pattern), x => x[1]);
        for(let m of matches) {aba_matches.push(m);}
    }
    if(aba_matches.length === 0) {return false;}

    const bab_patterns = aba_matches.map(m => m[1] + m[0] + m[1]);
    let bab_matched = false;
    for(const hypernet of addy.hypernet) {
        for(const bab_pattern of bab_patterns) {
            bab_matched ||= hypernet.includes(bab_pattern);
        }
    }
    return bab_matched;
}


const addys = fs.readFileSync(0, "ascii").split("\n")
      .filter(s => s.length > 0).map(splitAddy);
let tls_count = 0;
let ssl_count = 0;
for(const addy of addys) {
    if(supportsTls(addy)) {tls_count++;}
    if(supportsSsl(addy)) {ssl_count++;}
}

console.log(`TLS Support: ${tls_count}\nSSL Support: ${ssl_count}`);
