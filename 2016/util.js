#!/usr/bin/env node
"use strict";


class TreeNode {
    data;
    children = [];
    parent = null;
    #depth;

    constructor(data=null) {
        this.data = data;
    }

    toString() {
        let s = `Node ${this.data}`;
        if(this.children.length > 0) {
            s += " => ";
            s += this.children.map(child => `Node ${child.data}`).join(", ");
        }
        return s;
    }

    addChild(node) {
        node.parent = this;
        this.children.push(node);
        return node;
    }

    depth() {
        if(!this.#depth) {
            this.#depth = this.parent
                           ? this.parent.depth() + 1
                           : 0;
        }
        return this.#depth;
    }
}
exports.TreeNode = TreeNode;


function bfs(root, predicate, postPredHook) {
    let q = [root];
    let explored = [];
    while(q.length > 0) {
        let cur = q.pop();
        explored.push(cur);

        if(predicate(cur)) {
            return cur;
        }

        if(typeof postPredHook === "function") {
            postPredHook(cur);
        }

        for(const child of cur.children) {
            // TODO: [].includes compares with === and object references are
            // equal only if they point to the exact same object. TreeNode.eq()
            // or similar should probably be used instead for comparison.
            if(!explored.includes(child)) {
                q.unshift(child);
            }
        }
    }
    return null;
}
exports.bfs = bfs;


function dfs(root, predicate) {
    let stack = [root];
    let explored = [];

    while(stack.length > 0) {
        let cur = stack.pop();

        if(predicate(cur)) {
            return cur;
        }
        // TODO: [].includes compares with === and object references are equal
        // only if they point to the exact same object. TreeNode.eq() or
        // similar should probably be used instead for comparison.
        if(!explored.includes(cur)) {
            explored.push(cur);
            for(const child of cur.children) {
                stack.push(child);
            }
        }
    }
    return null;
}
exports.dfs = dfs;


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
exports.buildTree = buildTree;


class Pqueue {
    // Could extend Array, but then we are stuck implementing a lot of
    // methods that we don't need.
    array = [];
    dirty = true;

    push(elem, priority) {
        if(typeof priority !== "number") {
            throw new TypeError("expected a number for priority.");
        }

        this.dirty = true;
        this.array.push({data: elem, priority: priority});
        return this.array.length;
    }

    // lower numerical scores have higher priority and get popped first.
    pop() {
        if(this.dirty) {
            this.array.sort((a, b) => b.priority - a.priority);
            this.dirty = false;
        }
        return this.array.pop().data;
    }

    empty() {
        return this.array.length === 0;
    }

    length() {
        return this.array.length;
    }
}
exports.Pqueue = Pqueue;
