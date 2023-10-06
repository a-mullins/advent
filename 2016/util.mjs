#!/usr/bin/env node
"use strict";


class Deque {
    a;
    size = 0;
    maxSize;
    front = 0;
    back = 1;

    constructor(size) {
        this.maxSize = size;
        this.a = new Uint32Array(this.maxSize);
    }

    isFull() {
        return this.size === this.maxSize;
    }

    isEmpty() {
        return this.size === 0;
    }

    length() {
        return this.size;
    }

    push(elem) {
        if(this.isFull()) {
            throw new Error("full");
        }

        this.a[this.back] = elem;
        this.back = (this.back+1) % this.maxSize;
        return ++this.size;
    }

    unshift(elem) {
        if(this.isFull()) {
            throw new Error("full");
        }

        this.a[this.front] = elem;
        if(--this.front < 0) { this.front += this.maxSize; }
        return ++this.size;
    }

    pop() {
        if(this.isEmpty()) {
            return undefined;
        }

        this.size -= 1;
        if(--this.back < 0) { this.back += this.maxSize; }

        return this.a[this.back];
    }

    shift() {
        if(this.isEmpty()) {
            return undefined;
        }

        this.size -= 1;
        this.front = (this.front+1) % this.maxSize;

        return this.a[this.front];
    }

    //toString() {
    //    return this.#a.slice(0, this.#back+1).toString();
    //}
}


class Pqueue {
    // This solution is faster than a js-native heap for the 2017 AoC problems.
    // I checked.
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

    includes(elem) {
        return this.array.includes(elem);
    }
}


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
            //
            // Maybe we should take a function like Array.find().
            if(!explored.includes(child)) {
                q.unshift(child);
            }
        }
    }
    return null;
}


export { Deque, Pqueue, TreeNode, bfs};
