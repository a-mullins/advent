#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "../lib/darray.h"


#define NAME_CAP 12


typedef struct reg {
    char name[NAME_CAP];
    int val;
} reg;


typedef enum oper { GT, LT, GE, LE, EQ, NE } oper;
char oper_s[6][3] = {">", "<", ">=", "<=", "==", "!="};


typedef struct cond {
    reg *reg;
    oper oper;
    int val;
} cond;


typedef struct instr {
    reg *target;
    bool inc;
    int amt;
    cond cond;
} instr;


int
regcmp(const void *a, const void *b)
{
    return strcmp(((const reg *)a)->name, ((const reg *)b)->name);
}


void
strtoinstr(char *s, instr *i, darray *registers)
{
    char *tok = strtok(s, " ");
    i->target = darray_bsearch(registers, tok, regcmp);

    tok = strtok(NULL, " ");
    if(strcmp(tok, "inc") == 0) {
        i->inc = true;
    } else {
        i->inc = false;
    }

    tok = strtok(NULL, " ");
    i->amt = atoi(tok);

    tok = strtok(NULL, " ");
    /* discard */

    tok = strtok(NULL, " ");
    i->cond.reg = darray_bsearch(registers, tok, regcmp);

    tok = strtok(NULL, " ");
    if(!strcmp(tok, ">"))  {i->cond.oper = GT;}
    if(!strcmp(tok, "<"))  {i->cond.oper = LT;}
    if(!strcmp(tok, ">=")) {i->cond.oper = GE;}
    if(!strcmp(tok, "<=")) {i->cond.oper = LE;}
    if(!strcmp(tok, "==")) {i->cond.oper = EQ;}
    if(!strcmp(tok, "!=")) {i->cond.oper = NE;}

    tok = strtok(NULL, " ");
    i->cond.val = atoi(tok);
}


int
max_reg(darray *regs)
{
    int max = ((reg *)darray_get(regs, 0))->val;
    for (size_t i = 1; i < regs->len; i++) {
        int other = ((reg *)darray_get(regs, i))->val;
        if (other > max) {max = other;}
    }
    return max;
}


int
main(void)
{
    darray regs;
    darray_init(&regs, sizeof (reg));

    if(fseek(stdin, 0, SEEK_SET) != 0) {
        fprintf(stderr, "ERR\n"); exit(1);
    }

    char *line = NULL;
    size_t len = 0;
    while (getline(&line, &len, stdin) >= 0) {
        reg r = {0};
        char *tok = strtok(line, " ");
        strncpy(r.name, tok, NAME_CAP-1);
        if(darray_in(&regs, &r) == -1) {
            darray_push(&regs, &r);
        }
    }
    free(line); line = NULL; len = 0;

    // So we can use darray_bsearch later.
    darray_qsort(&regs, regcmp);
    rewind(stdin);
    int all_time_high = 0;
    while (getline(&line, &len, stdin) >= 0) {
        instr i = {0};
        strtoinstr(line, &i, &regs);

        reg *cond_lhs = i.cond.reg;
        bool test = false;
        switch (i.cond.oper) {
        case GT:
            test = cond_lhs->val > i.cond.val;
            break;
        case LT:
            test = cond_lhs->val < i.cond.val;
            break;
        case GE:
            test = cond_lhs->val >= i.cond.val;
            break;
        case LE:
            test = cond_lhs->val <= i.cond.val;
            break;
        case EQ:
            test = cond_lhs->val == i.cond.val;
            break;
        case NE:
            test = cond_lhs->val != i.cond.val;
            break;
        }
        if (test) {
            if (i.inc) {
                i.target->val += i.amt;
            } else {
                i.target->val -= i.amt;
            }
        }
        int cur_max = max_reg(&regs);
        if (cur_max > all_time_high) {all_time_high = cur_max;}
    }
    free(line); line = NULL; len = 0;

    printf("all time high: %d, max at end: %d\n", all_time_high, max_reg(&regs));

    // Cleanup.
    darray_free(&regs, NULL);
    return 0;
}
