#!/usr/bin/env node
//"use strict";
//var fs = require("fs");

//var codes = fs.readFileSync(0, 'ascii').split('\n').map( line => line.split('') ).filter(line => line.length > 0);
// This seems like a hack. The intention seems to be that Array.from()
// will use the .length proprety of an Array-like to access elements
// obj[0]...obj[length-1] & call the supplied fn on them. ofc,
// property 0 of a new object is undefined so...
// Other methods are commented below.
//var counts = Array.from({length: 6}, () => new Map());
// var counts = new Array();
// for(let i=0; i<6; i++) { counts.push(new Map()); }
// var counts = new Array(6).fill(new Map());   THIS DOES NOT DO THE EXPECTED



// !!! THIS CODE DOES NOT DO WHAT IS EXPECTED AND DEMONSTRATES
// !!! MULTIPLE JS FOOTGUNS. DO NOT USE.
//
// I think the confusion arrises from the fact that Array uses [] to access
// values by index, but Map does not mimic this behavior. Additionally,
// many other languages use [] to access elements of a map/a-array/dict
// for(let row = 0; row < codes.length; row++) {
//     for(let col = 0; col < 6; col++) {
//         let letter = codes[row][col];
//         if(letter in counts[col]) {        // THIS DOES NOT DO THE EXPECTED
//             counts[col][letter] += 1;      // THIS DOES NOT DO THE EXPECTED
//         } else {
//             counts[col][letter] = 1;       // THIS DOES NOT DO THE EXPECTED
//         }
//     }
// }

//array = [0, 2, 1, 5, 3, 9, 15, 4, 8];
//array.reduce( (prev, cur) => cur > prev ? cur : prev )

// TODO blog post about how map[key] = value doesn't do what is expected.
// (why not just overload the [] operator? Or throw an exception?)
