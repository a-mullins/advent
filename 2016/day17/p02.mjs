#!/usr/bin/env node
// Copyright (c) 2025 Adam Mullins
//
// Solution for Advent of Code 2016, Day 17, Part 2
//   https://adventofcode.com/2016/day/17#part2
"use strict";
import fs from "node:fs";
import crypto from "node:crypto";
import { TreeNode } from "../util.mjs";


// root :: TreeNode
// goalPred :: function(TreeNode) -> Boolean
// makeChildren :: function(TreeNode) -> _
function* buildTree(root, goalPred, makeChildren) {
    let stack = [root];
    while(stack.length > 0) {
        let cur = stack.pop();
        if(goalPred(cur)) {
            yield cur;
        } else {
            makeChildren(cur);
            for(const child of cur.children) {
                stack.push(child);
            }
        }
    }
}


const seed = "bwnlcvfs";
//const seed = fs.readFileSync(0, "ascii").trim();
const root = new TreeNode({label: seed, coord: {x: 0, y: 0}});


// room :: {label: "", coord: {x: int, y: int}}
function makeNextRooms(room) {
    makeNextRooms.open = makeNextRooms.open || "bcdef";

    let hash = crypto.createHash("md5").update(room.label).digest("hex");
    const nextRooms = [];

    // Check UP.
    if(room.coord.y - 1 >= 0 && makeNextRooms.open.includes(hash[0])) {
        nextRooms.push({label: room.label + "U",
                        coord: {x: room.coord.x, y: room.coord.y - 1}});
    }
    // Check DOWN.
    if(room.coord.y + 1 <= 3 && makeNextRooms.open.includes(hash[1])) {
        nextRooms.push({label: room.label + "D",
                        coord: {x: room.coord.x, y: room.coord.y + 1}});
    }
    // Check LEFT.
    if(room.coord.x - 1 >= 0 && makeNextRooms.open.includes(hash[2])) {
        nextRooms.push({label: room.label + "L",
                        coord: {x: room.coord.x - 1, y: room.coord.y}});
    }
    // Check RIGHT.
    if(room.coord.x + 1 <= 3 && makeNextRooms.open.includes(hash[3])) {
        nextRooms.push({label: room.label + "R",
                        coord: {x: room.coord.x + 1, y: room.coord.y}});
    }

    return nextRooms;
}


function isGoalNode(node) {
    return node.data.coord.x === 3 && node.data.coord.y === 3;
}


function makeChildren(node) {
    for(const room of makeNextRooms(node.data)) {
        node.addChild(new TreeNode(room));
    }
}


const paths = Array.from(
    buildTree(root, isGoalNode, makeChildren),
    node => node.data.label);
console.log(
    paths.reduce((acc, cur) => Math.max(cur.length, acc), 0) - seed.length
);
