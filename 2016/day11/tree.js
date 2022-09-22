#!/usr/bin/env node

class Node {
    constructor(data, edges) {
        this.data = (data == undefined ? null : data);
        this.edges = [];
        if(edges) {
            edges.forEach(edge => this.edges.push(edge));
        }
    }

    toString() {
        let output = `Node ${this.data}`;
        if(this.edges.length > 0) {
            output += ' => ';
            output += this.edges.map( child => child.data ).join(', ');
        }
        return output;
    }
}
exports.Node = Node;

function bfs(root, predicate) {
    if(!(root instanceof Node)) { throw new TypeError('root is not a Node');}
    let q = [root];
    let explored = [root];
    while(q.length > 0) {
        let cur = q.pop();
//        console.log('visiting %d', cur.data);
        if( predicate(cur) ) {
            return cur;
        } else {
            for(let child of cur.edges) {
                if(!explored.includes(child)) {
                    explored.unshift(child);
                    q.unshift(child);
                }
            }
        }
    }
    return null;
}
exports.bfs = bfs;

function dfs(root, predicate) {
    if(!(root instanceof Node)) { throw new TypeError('root is not a Node');}
    let stack = [root];
    let explored = [];
    while(stack.length > 0) {
        let cur = stack.pop();
//        console.log('visiting %d', cur.data);
        if(predicate(cur)) {
            return cur;
        } else {
            if(!explored.includes(cur)) {
                explored.push(cur);
                for(let edge of cur.edges) {
                    stack.push(edge);
                }
            }
        }
    }
    return null;
}
exports.dfs = dfs;

// A test tree.
// var n0 = new Node(0, [new Node(2), new Node(4)]);
// n0.edges[0].edges.push(new Node(3), new Node(5));
// n0.edges[1].edges.push(new Node(11), new Node(13));
