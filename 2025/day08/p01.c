#include <float.h>
#include <math.h>
#include <stdio.h>
#include "../lib/darray.h"


typedef struct {
    long x;
    long y;
    long z;
} vec3;


float dist(vec3 u, vec3 v);
//ssize_t find_circuit_by_pos(struct darray *circuits, vec3 pos);
// boxes :: [vec3, vec3, ...],
// pairs :: [(vec3, vec3), (vec3, vec3)...]
// pairs is an out-param.
void find_n_closest_pairs(struct darray *boxes, struct darray *pairs, size_t n);
void print_circuit(struct darray *d);
void add_box_to_target_circuits(struct darray *circuits, vec3 target, vec3 box);
void circuits_free(void *d);


int
main(void)
{
    struct darray *boxes = darray_new(sizeof (vec3)); // this could be array.
    struct darray *pairs = darray_new(sizeof (vec3) * 2);
    struct darray *circuits = darray_new(sizeof (struct darray *));


    char line[128];
    while (NULL != fgets(line, 128, stdin)) {
        vec3 v;
        sscanf(line, "%ld,%ld,%ld", &v.x, &v.y, &v.z);
        darray_push(boxes, &v);
    }
    struct darray *unconnected = darray_copy(boxes);
    // List of unconnected boxes.
    // printf("%ld junctions unconnected:\n", darray_len(boxes));
    // print_circuit(boxes);
    // puts("");

    find_n_closest_pairs(boxes, pairs, 10);
    for (size_t i = 0; i<10; i++) {
        vec3 *v_p = darray_get(pairs, i);
        add_box_to_target_circuits(circuits, v_p[0], v_p[1]);
        darray_del_val(unconnected, &(v_p[0]));
        darray_del_val(unconnected, &(v_p[1]));
    }

    // List of unconnected boxes.
    printf("%ld junctions unconnected:\n", darray_len(unconnected));
    print_circuit(unconnected);
    puts("");
    printf("%ld non-singleton circuits.\n", darray_len(circuits));
    for (size_t i = 0; i < darray_len(circuits); i++) {
        struct darray *circ = *(struct darray **)darray_get(circuits, i);
        printf("circuit %ld has %ld boxes:\n", i, darray_len(circ));
        print_circuit(circ);
    }

    darray_free(circuits, circuits_free);
    darray_free(pairs, NULL);
    darray_free(boxes, NULL);
    return 0;
}


float
dist(vec3 u, vec3 v)
{
    return sqrtf(
        powf(((float)u.x - (float)v.x), 2) +
        powf(((float)u.y - (float)v.y), 2) +
        powf(((float)u.z - (float)v.z), 2) );
}


// ssize_t
// find_circuit_by_pos(struct darray *circuits, vec3 pos)
// {
//     for (ssize_t i = 0; i < darray_len(circuits); i++)
//         if (darray_in(*(struct darray**)darray_get(circuits, i), pos) >= 0)
//             return i;
//     return -1;
// }


// boxes :: [vec3, vec3, ...],
// pairs :: [(vec3, vec3), (vec3, vec3)...], ie, [vec3 *, vec3 *]
// pairs is an out-param.
void
find_n_closest_pairs(struct darray *boxes, struct darray *pairs, size_t n)
{
    for (size_t i = 0; i < n; i++) {
        vec3 v0, v1;
        float min = FLT_MAX;
        float lower_bound;
        if (darray_last(pairs)) {
            vec3 *v_p0 = darray_last(pairs);
            lower_bound = dist(v_p0[0], v_p0[1]);
        } else {
            lower_bound = 0.0f;
        }
        for (size_t i = 0; i<darray_len(boxes); i++) {
            for (size_t j = i + 1; j<darray_len(boxes); j++) {
                vec3 maybe0 = *(vec3 *)darray_get(boxes, i);
                vec3 maybe1 = *(vec3 *)darray_get(boxes, j);
                float d = dist(maybe0, maybe1);
                if (d > lower_bound && d < min) {
                    v0 = maybe0;
                    v1 = maybe1;
                    min = d;
                }
            }
        }
        vec3 v[2] = {v0, v1};
        darray_push(pairs, v);
    }
    return;
}


// circuits :: [darray *, darray *, ...], equivalently,
// circuits :: [[vec3], [vec3]]
// remember that darray_get() returns a pointer to the elem, so do
// darray *circ = *(darray **)darray_get(circuits, ...)
void
add_box_to_target_circuits(struct darray *circuits, vec3 target, vec3 box)
{
    struct darray *target_circ = NULL;
    for (ssize_t i = 0; i < darray_len(circuits); i++) {
        struct darray *tmp = *(struct darray**)darray_get(circuits, i);
        if (darray_in(tmp, &target) >= 0)
            target_circ = tmp;
    }
    if (target_circ) {
        darray_push(target_circ, &box);
    } else {
        target_circ = darray_new(sizeof (vec3));
        darray_push(target_circ, &target);
        darray_push(target_circ, &box);
        darray_push(circuits, &target_circ);
    }

    return;
}

// void
// join_closest_boxes(struct darray *unconnected, struct darray *circuits)
// {
//     size_t a, b;
//     float min = FLT_MAX;
//     for (size_t i = 0; i<darray_len(unconnected); i++) {
//         for (size_t j = i + 1; j<darray_len(unconnected); j++) {
//             float d = dist(*(vec3 *)darray_get(unconnected, i),
//                            *(vec3 *)darray_get(unconnected, j));
//             if (d < min) {
//                 a = i;
//                 b = j;
//                 min = d;
//             }
//         }
//     }
//     vec3 v0;
//     vec3 v1;


//     // printf("%ld and %ld are closest at %.1f units.\n", a, b, min);
//     // // array of pointers to dynamic arrays.
//     // struct darray *circ = darray_new(sizeof (vec3));
//     // vec3 v0 = *(vec3 *)darray_get(unconnected, a);
//     // vec3 v1 = *(vec3 *)darray_get(unconnected, b);
//     // darray_push(circ, &v0);
//     // darray_push(circ, &v1);
//     // darray_push(circuits, &circ);
//     // // darray_del_id(unconnected, darray_in(unconnected, &v0));
//     // // darray_del_id(unconnected, darray_in(unconnected, &v1));
//     // darray_del_val(unconnected, &v0);
//     // darray_del_val(unconnected, &v1);

//     // printf("%05ld %05ld %05ld is in circuit %ld\n",
//     //        v0.x, v0.y, v0.z,
//     //        find_circuit_by_pos(circuits, &v0));
//     // printf("%05ld %05ld %05ld is in circuit %ld\n",
//     //        v1.x, v1.y, v1.z,
//     //        find_circuit_by_pos(circuits, &v1));

//     // printf("\ncircuit 0 has elems\n");
//     // for (size_t i = 0; i<darray_len(circ); i++) {
//     //     vec3 *v_p = (vec3 *)darray_get(circ, i);
//     //     printf("%05ld %05ld %05ld\n", v_p->x, v_p->y, v_p->z);
//     // }
// }


// ssize_t
// find_circuit_by_box(struct circuit_darr *circuits, vec3 box)
// {
//     // for vec3_darr in circuits...
//     for (size_t i = 0; i < circuit_darr_len(circuits); i++)
//         // is this box in this vec3_darr?
//         if (vec3_darr_in(circuit_darr_get(circuits, i), box) > -1)
//             return i;
//     return -1;
// }


void
print_circuit(struct darray *d)
{
    for (size_t i = 0; i<darray_len(d); i++) {
        vec3 *v_p = (vec3 *)darray_get(d, i);
        printf("%5ld %5ld %5ld\n", v_p->x, v_p->y, v_p->z);
    }
}


void
circuits_free(void *d)
{
    darray_free(d, NULL);
    return;
}
