#!/usr/bin/env node
"use strict";
const crypto = require("crypto");
const fs = require("fs");
const util = require("../util");

const seed = fs.readFileSync(0, "ascii").trim();
const root = new util.TreeNode({label: seed, coord: {x: 0, y: 0}});


// room :: {label: "", coord: {x: int, y: int}}
function makeNextRooms(room) {
    makeNextRooms.open = makeNextRooms.open || "bcdef"; // fewer mallocs?

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
        node.addChild(new util.TreeNode(room));
    }
}


const goal = util.bfs(root, isGoalNode, makeChildren);
console.log(`Exit found in ${goal.depth()} steps.\n`+
            `Path is: ${goal.data.label.slice(seed.length)}`);
