#!/usr/bin/env node
"use strict";

var fs = require("fs");


// lines have the form of either:
// value N goes to bot B
// bot B gives low to (bot|output) L and high to (bot|output) H
var lines  = fs.readFileSync(0, 'ascii').trim().split('\n');
var rules  = lines.filter(line => line.includes('gives'));
var inputs = lines.filter(line => line.includes('goes'));

//wip
