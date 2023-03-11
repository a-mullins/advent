#!/usr/bin/env node
"use strict";
const fs = require("fs");
const util = require("../util");
// TODO: rewrite without using TreeNode.


function gridVal(x, y) {
    let num = x*x + 3*x + 2*x*y + y + y*y + fav;
    let one_count = 0;
    while(num > 0) {
        if(num % 2) {one_count++;}
        num = Math.floor(num / 2);
    }

    return (one_count % 2 ? "#" : ".");
}


function openNeighbors(point) {
    //console.log(`Open neighbors of ${point.x}, ${point.y}.`);
    let points = [
         {x: point.x-1, y: point.y}
        ,{x: point.x+1, y: point.y}
        ,{x: point.x, y: point.y-1}
        ,{x: point.x, y: point.y+1}
    ].filter(p => p.x >= 0 && p.y >= 0);
    points.forEach(p => p.val = gridVal(p.x, p.y));

    //for(let p of points) {console.log(`\t(${p.x}, ${p.y})->${p.val}`);}
    points = points.filter(p=>p.val === ".");
    //console.log();
    //for(let p of points) {console.log(`\t(${p.x}, ${p.y})->${p.val}`);}

    return points;
}


function taxiDist(p1, p2) {
    return Math.abs(p1.x - p2.x) + Math.abs(p1.y - p2.y);
}


const fav = 1364;
const goal = {x: 31, y: 39};
const explored = [];
const frontier = new util.Pqueue();
frontier.push(new util.TreeNode({x:1, y:1}), 0);

while(!frontier.empty()) {
    const cur_node = frontier.pop();
    explored.push(cur_node.data);
    //console.log(`Considering ${cur_node.data.x}, ${cur_node.data.y}.`);

    if(cur_node.data.x === goal.x && cur_node.data.y === goal.y) {
        console.log(cur_node.depth());
        break;
    }

    for(const open of openNeighbors(cur_node.data)) {
        if(explored.find(elem => elem.x === open.x && elem.y === open.y)) {
            //console.log(`\tAlready seen (${open.x}, ${open.y}) => ${open.val}`);
            continue;
        }

        const child = new util.TreeNode(open);
        cur_node.addChild(child);
        frontier.push(child, cur_node.depth() + taxiDist(open, goal));
    }
}
