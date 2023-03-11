#!/usr/bin/env node
"use strict";
const fs = require("fs");
const util = require("../util");
// TODO: rewrite to avoid TreeNode.


function isPointOpen(p) {
    let num = p.x*p.x + 3*p.x + 2*p.x*p.y + p.y + p.y*p.y + 1364;
    let one_count = 0;
    while(num > 0) {
        if(num % 2) {one_count++;}
        num = Math.floor(num / 2);
    }

    //return (one_count % 2 ? "#" : ".");
    return !(one_count % 2);
}


function openNeighbors(point) {
    //console.log(`Open neighbors of ${point.x}, ${point.y}.`);
    return [
        {x: point.x-1, y: point.y}
        ,{x: point.x+1, y: point.y}
        ,{x: point.x, y: point.y-1}
        ,{x: point.x, y: point.y+1}
    ].filter(p => p.x >= 0 && p.y >= 0).filter(p=>isPointOpen(p));
}


function taxiDist(p1, p2) {
    return Math.abs(p1.x - p2.x) + Math.abs(p1.y - p2.y);
}


function search(start, end) {
    const explored = [];
    const frontier = new util.Pqueue();
    frontier.push(new util.TreeNode(start), 0);

    while(!frontier.empty()) {
        const cur_node = frontier.pop();
        explored.push(cur_node.data);

        if(cur_node.data.x === end.x && cur_node.data.y === end.y) {
            return cur_node.depth();
        }

        for(const open of openNeighbors(cur_node.data)) {
            if(explored.find(elem => elem.x === open.x && elem.y === open.y)
               || taxiDist(open, end) > 50) {
                //console.log(`\tAlready seen (${open.x}, ${open.y}) => ${open.val}`);
                continue;
            }

            const child = new util.TreeNode(open);
            cur_node.addChild(child);
            frontier.push(child, cur_node.depth() + taxiDist(open, end));
        }
    }

    return null;
}


let points = [];
for(let x = 0; x <= 50; x++) {
    for(let y = 0; y <= 50; y++) {
        points.push({x: x, y: y});
    }
}

console.log(points
            .map(p => search({x: 1, y: 1}, p))
            .filter(p => p !== null && p <= 50)
            .length);
