#!/usr/bin/env node
"use strict";


const fs = require('fs');


function splitAddy(s) {
    let parts = {supernet: [], hypernet: []}
    let start = 0;
    for(let i = 0; i < s.length; i++) {
        if(i+1 == s.length && s[i] != ']') {
            parts.supernet.push(s.slice(start, i+1));
        }
        if(s[i] == '[') {
            parts.supernet.push(s.slice(start, i));
            start = i+1;
        }
        if(s[i] == ']') {
            parts.hypernet.push(s.slice(start, i));
            start = i+1;
        }
    }
    return parts;
}

function supportsTls(addy) {
    function _isAbba(s) {
        return s[0] != s[1] && s[0] == s[3] && s[1] == s[2];
    }
    function _hasAbba(s) {
        let found = false;
        for(let i = 0; i+4 <= s.length; i++) {
            found ||= _isAbba(s.slice(i, i+4));
        }
        return found;
    }

    return addy.supernet.some(_hasAbba) && !(addy.hypernet.some(_hasAbba));
}

function supportsSsl(addy) {
    // regex abuse to find overlapping matches:
    let aba_pattern = /(?=((\w)(?!\2).\2))/gi;
    let aba_matches = [];
    for( let supernet of addy.supernet ) {
        let matches = Array.from( supernet.matchAll(aba_pattern), x => x[1] );
        for( let m of matches ) { aba_matches.push(m); }
    }
    if( aba_matches.length == 0 ) { return false; }

    let bab_patterns = aba_matches.map( m => m[1] + m[0] + m[1]);
    let bab_matched = false;
    for( let hypernet of addy.hypernet ) {
        for( let bab_pattern of bab_patterns ) {
            bab_matched ||= hypernet.includes(bab_pattern);
        }
    }
    return bab_matched;
}


let lines = fs.readFileSync(0, 'ascii').split('\n').filter(s => s.length > 0);
let addys = lines.map( splitAddy );
let tls_count = 0,
    ssl_count = 0;
for( let addy of addys ) {
    if( supportsTls(addy) ) {
        tls_count++;
    }
    if( supportsSsl(addy) ) {
        ssl_count++
    }
}

console.log("TLS Support: %s\nSSL Support: %s", tls_count, ssl_count);
