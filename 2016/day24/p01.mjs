#!/usr/bin/env node
// Copyright (c) 2025 Adam Mullins
//
// Solution for Advent of Code 2016, Day 24, Part 1
//   https://adventofcode.com/2016/day/24
"use strict";
import fs from "node:fs";
import { Pqueue } from "../util.mjs";


// TODO examine permute()
function permute(array) {
    const result = [];

    function _permute(arr, m = []) {
        if(arr.length === 0) {
            result.push(m);
        } else {
            for(let i = 0; i < arr.length; i++) {
                let curr = arr.slice();
                let next = curr.splice(i, 1);
                _permute(curr.slice(), m.concat(next));
            }
        }
    };

    _permute(array);

    return result;
}


function pairs(xs) {
    const output = [];
    for(let i = 1; i < xs.length; i++) {
        output.push([xs[i-1],  xs[i]]);
    }
    return output;
}


function taxi_dist(start, goal) {
    // start & goal are ints that represent node index
    const p1x = start % num_cols;
    const p1y = Math.floor(start / num_cols);
    const p2x = goal % num_cols;
    const p2y = Math.floor(goal / num_cols);
    return Math.abs(p1x - p2x) + Math.abs(p1y - p2y);
}


function neighbors(node) {
    const maybe_neighbors = [
        node - 1,
        node + 1,
        node - num_cols,
        node + num_cols
    ];
    return maybe_neighbors.filter(n => base_map[n]);
}


function reconstruct_path(came_from, current) {
    const total_path = [current];
    while(current in came_from) { // current in came_from.keys
        current = came_from[current];
        total_path.unshift(current);
    }
    return total_path;
}


// A*.
//
// Inspired by: en.wikipedia.org/wiki/A*_search_algorithm
// Notes:
//     n is the next node on the path.
//     g(n) is the cost of the path from start to n
//     h(n) is the estimate of the cheapest path from n the the goal.
//          (h should be admissable!)
//     f(n) = g(n) + h(n)
//     f(n) is the best guest for how cheap a path is from start to goal if it
//          goes through n.
function a_star(start, goal, h) {
    // start :: int        (node id#)
    //  goal :: int        (node id#)
    //     h :: int -> int

    const frontier = new Pqueue();
    frontier.push(start, 0);

    // For node n, came_from[n] is the node immediately preceding it on the
    // cheapest path from the start to n currently known.
    const came_from = [];

    const g_score = new Uint32Array(num_rows * num_cols);
    g_score.fill(2**32 - 1); // default to 'infinity'
    g_score[start] = 0;      // from start to start costs 0.

    const f_score = new Uint32Array(num_rows * num_cols);
    f_score.fill(2**32 - 1); // default to 'infinity'
    f_score[start] = h(start);

    while(!frontier.empty()) {
        const current = frontier.pop();
        if(current === goal) {
            return reconstruct_path(came_from, current);
        }

        for(const neighbor of neighbors(current)) {
            // + 1 because its costs one more hop to get from here to there.
            // Other implementations might use d(current, neighbor).
            let tentative_g_score = g_score[current] + 1;
            let neighbor_g_score = g_score[neighbor];
            if(tentative_g_score < g_score[neighbor]) {
                // If the path we took to get here is cheaper than the
                // currently known path, record it and note where we came from
                // to get here.
                came_from[neighbor] = current;
                g_score[neighbor] = tentative_g_score;
                f_score[neighbor] = tentative_g_score + h(neighbor);
                if(!frontier.includes(neighbor)){
                    frontier.push(neighbor, f_score[neighbor]);
                }
            }
        }
    }
    // Open set is empty but goal was never reached
    return null;
}


function map_to_string() {
    let output = [];

    for(let row = 0; row < (base_map.length / num_cols); row++) {
        for(let col = 0; col < num_cols; col++) {
            if(base_map[col + row*num_cols]) {
                output.push(".");
            } else {
                output.push("#");
            }
        }
        output.push("\n");
    }

    return output.join("");
}


const input = fs.readFileSync(0, "ascii").trim().split("\n");
var num_rows = input.length;
var num_cols = input[0].length;
const bookmarks = [];
// 0 if wall or visited, 1 otherwise. Bookmarks saved seperately.
var base_map = new Uint8Array(num_rows * num_cols);
base_map.fill(1);

for(let row = 0; row < num_rows; row++) {
    for(let col = 0; col < num_cols; col++) {
        // 1. Wall
        if(input[row][col] == "#") {
            base_map[col + row*num_cols] = 0;
        }
        // 2. Bookmark: don't change base_map, but save locaction
        if(input[row][col].match(/\d/)) {
            bookmarks[Number(input[row][col])] = col + row*num_cols;
        }
        // 3. Open space: do nothing, the value is already 0.
    }
}

const all_paths = permute(bookmarks.slice(1)).
      map(xs => bookmarks.slice(0,1).concat(xs));
const all_dists = [];
for(const path of all_paths) {
    let all_pairs = pairs(path);
    let acc = 0;
    for(const p of all_pairs) {
        let d = a_star(p[0], p[1], x=>taxi_dist(p[0], x)).length-1;
        acc += d;
    }
    all_dists.push(acc);
}
console.log(Math.min(...all_dists));
