// TODO remove debug stuff
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "../darray.h"


#define NAME_CAP 12


typedef struct reg {
    char name[NAME_CAP];
    int val;
} reg;


typedef enum oper { GT, LT, GE, LE, EQ, NE } oper;
char oper_s[6][3] = {">", "<", ">=", "<=", "==", "!="};


typedef struct cond {
    char reg[NAME_CAP];
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
    strncpy(i->cond.reg, tok, NAME_CAP-1);

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


char *
dump_regs(char **s, darray *registers) {
    char buf[512] = {0};
    size_t end = 0;

    for(size_t i = 0; i < registers->len; i++) {
        reg *r = (reg *)darray_get(registers, i);
        end += snprintf(buf+end,
                        512 - end,
                        "%3s: %5d, ",
                        r->name, r->val);
    }

    buf[end - 2] = '\0';
    *s = calloc(strlen(buf)+1, sizeof (char));
    strcpy(*s, buf);

    return *s;
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
    int linum = 0;
    char *foo = NULL;
    printf("                                      ");
    puts(dump_regs(&foo, &regs));
    free(foo);
    while (getline(&line, &len, stdin) >= 0) {
        instr i = {0};
        strtoinstr(line, &i, &regs);

        reg *cond_lhs = (reg *)darray_bsearch(&regs, i.cond.reg, regcmp);
        bool succ = false;
        printf("%04d: %3s %s %5d if %3s %2s %5d",
               ++linum, i.target->name, i.inc ? "inc" : "dec", i.amt,
               cond_lhs->name, oper_s[i.cond.oper], i.cond.val);
        switch (i.cond.oper) {
        case GT:
            succ = cond_lhs->val > i.cond.val;
            break;
        case LT:
            succ = cond_lhs->val < i.cond.val;
            break;
        case GE:
            succ = cond_lhs->val >= i.cond.val;
            break;
        case LE:
            succ = cond_lhs->val <= i.cond.val;
            break;
        case EQ:
            succ = cond_lhs->val == i.cond.val;
            break;
        case NE:
            succ = cond_lhs->val != i.cond.val;
            break;
        }
        if (succ) {
            if (i.inc) {
                i.target->val += i.amt;
            } else {
                i.target->val -= i.amt;
            }
        }
        printf(" | ");
        puts(dump_regs(&foo, &regs));
        free(foo);
    }
    free(line); line = NULL; len = 0;

    int max = ((reg *)darray_get(&regs, 0))->val;
    for(size_t i = 1; i < regs.len; i++) {
        int other = ((reg *)darray_get(&regs, i))->val;
        if(other > max) { max = other; }
    }

    printf("\nmax: %d\n", max);

    // Cleanup.
    darray_free(&regs);
    return 0;
}
