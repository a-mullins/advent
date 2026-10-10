#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include "alist.h"


struct heapq_entry {
	void *data;
	size_t pri;
};


struct heapq {
	struct alist *al;
};

struct heapq *
heapq_new(size_t elem_size)
{
	struct heapq *hqp = malloc(sizeof (struct heapq));
	hqp->al = alist_new(sizeof (struct heapq_entry));

	if (hqp->al == NULL) {
		free(hqp);
		hqp = NULL;
	}

	return hqp;
}


void
heapq_free(struct heapq *hq, void (*destruct)(void *elem))
{
	alist_free(hq->al, destruct);
}


size_t
heapq_len(struct heapq *hq)
{
        return alist_len(hq->al);
}


int invar_assert(struct heapq *hq); /*TODO*/


size_t
heapq_pri(struct heapq *hq, size_t idx)
{
	if (idx >= heapq_len(hq))
		return SIZE_MAX;
	
	return ((struct heapq_entry *)alist_get(hq->al, idx))->pri;
}


size_t
lchild_i(size_t idx)
{
        return 2*idx + 1;
}

size_t
lchild_pri(struct heapq *hq, size_t idx)
{
	if (lchild_i(idx) >= heapq_len(hq))
		return SIZE_MAX;
	
	return ((struct heapq_entry *)alist_get(hq->al, lchild_i(idx)))->pri;
}


size_t
rchild_i(size_t idx)
{
        return 2*idx + 2;
}


size_t
rchild_pri(struct heapq *hq, size_t idx)
{
	if (rchild_i(idx) >= heapq_len(hq))
		return SIZE_MAX;
	
	return ((struct heapq_entry *)alist_get(hq->al, rchild_i(idx)))->pri;
}


size_t
parent_i(size_t idx)
{
	/* 0 returns -1. index 0 has no parent. */
	return (idx-1)/2;
}


size_t
parent_pri(struct heapq *hq, size_t idx)
{
	return ((struct heapq_entry *)alist_get(hq->al, parent_i(idx)))->pri;
}


/* Is the vertex given by idx a lower priority than its children? */
int
is_vertex_ordered(struct heapq *hq, size_t idx)
{
	size_t parent;
	size_t left;
	size_t right;
		
	if (idx >= heapq_len(hq))
		return 1;

	parent = ((struct heapq_entry *)alist_get(hq->al, idx))->pri;
	left = lchild_pri(hq, idx);
	right = rchild_pri(hq, idx);
	
	return (parent <= left) && (parent <= right);
}


int
is_heap(struct heapq *hq) {
	int valid = 1;
	for (size_t i = 0; i < heapq_len(hq); i++) {
		valid &= is_vertex_ordered(hq, i);
	}
	return valid;
}


void *
heapq_get(struct heapq *hq, size_t idx)
{
	if (idx < 0)
		return NULL;
	if (idx >= alist_len(hq->al))
		return NULL;
	struct heapq_entry e = *(struct heapq_entry *)alist_get(hq->al, idx);
	return e.data;
}


void
_perc_up(struct heapq *hq, size_t idx)
{
        //console.log(`_perc_up(${idx})`);
        // base case.
        if (idx == 0) {
		//console.log("base-case");
		return;
        } else {
		// console.log(`parent_elem.priority > elem.priority: `+
		//             `${parent_elem.pri > elem.pri}`);
		if(parent_pri(hq, idx) > heapq_pri(hq, idx)) {
			// if parent's priority is higher, swap parent and this
			// console.log("parent's priority is higher than child's");
			// console.log(this.array);

			alist_swap(hq->al, idx, parent_i(idx));
			
			//console.log(this.array);
			// now perc_up from new location
			_perc_up(hq, parent_i(idx));
		} else {
			// this element's priority is higher than its parent's, it can
			// stay put.
			//console.log("parent's priority is lower than child's");
			return;
		}
        }
    }

    /*
    _sink_down(idx) {
        //console.log(`_sink_down(${idx})`);
        //console.log(`this.elem_at(idx): ${this.elem_at(idx)}`);
        const elem = this.elem_at(idx) || {pri:-Number.MAX_SAFE_INTEGER};
        const lchild_i = this.lchild_i(idx);
        const rchild_i = this.rchild_i(idx);
        const lchild = this.elem_at(lchild_i) || {pri:Number.MAX_SAFE_INTEGER};
        const rchild = this.elem_at(rchild_i) || {pri:Number.MAX_SAFE_INTEGER};
        const smallest_i = rchild.pri < lchild.pri ? rchild_i : lchild_i;
        const smallest = this.elem_at(smallest_i);
        //console.log(`\tparent{ ${elem.pri} } =>`);
        //console.log("\t\t"+lchild.pri);
        //console.log("\t\t"+rchild.pri);

        // done when both children are >= this priority, or
        // there are no children and we are a leaf.
        if(lchild.pri >= elem.pri && rchild.pri >= elem.pri) {
            //console.log("\tbase-case");
            return;
        }
        // otherwise, swap this node with the smallest of its children & repeat.
        //console.log("\tidx, smallest_i, smallest.pri, elem.pri");
        //console.log(`\t${idx}, ${smallest_i}, ${smallest.pri}, ${elem.pri}`);
        //console.log(`this.array[${idx}] = ${this.elem_at(smallest_i)};`);
        this.array[smallest_i] = elem;
        this.array[idx] = smallest;
        this._sink_down(smallest_i);
        // if(lchild.pri < elem.pri) {
        //     //console.log("lchild.pri < elem.pri");
        //     //console.log(`${lchild.pri} < ${elem.pri}`);
        //     // swap lchild with this element
        //     this.array[lchild_i] = elem;
        //     this.array[idx] = lchild;
        //     this._sink_down(lchild_i);
        // } else if(rchild.pri < elem.pri) {
        //     // swap rchild with this element
        //     this.array[rchild_i] = elem;
        //     this.array[idx] = rchild;
        //     this._sink_down(rchild_i);
        // }
    }
*/

void
heapq_push(struct heapq *hq, void *elem, size_t priority)
{
        //console.log(`push(${elem}, ${priority}`);
        // insertion.
        // put elem at the first avail space, then rebalance tree
        // by percolating the element up.

	struct heapq_entry entry;
	/*
	 * FIXME this sequence results in two back-to-back calls
	 * to memcpy() for no good reason. :(
	 */
	/*
	 * FIXME This doesn't work. heapq_entry has a void * which needs to point
	 * to malloced memory to mean anything.
	 */
	memcpy(&(entry.data), elem, hq->al->elem_size);
	entry.pri = priority;
        //this.array.push({data: elem, pri: priority});
	alist_push(hq->al, &entry);
        //console.log(this.array);

        //this._perc_up(this.array.length-1);
	_perc_up(hq, alist_len(hq->al) - 1);
        //this.invar_assert();
}
/*
    pop() {
        //console.log(this.array);
        if(this.empty()) { return null; }
        // extraction.
        // remove the root, insert last element in root spot.
        // balance tree by sink element down.
        const old_root = this.array[0];
        // move last element into root.
        this.array[0] = this.array[this.array.length-1];
        // "delete" last element.
        this.array.length--;

        // insert sort, basically.
        this._sink_down(0);

        //this.invar_assert();
        return old_root.data;
    }
*/
