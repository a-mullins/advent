#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// To find array size:
//   cut -d' ' -f1,5 <input | tr ' ' '\n' | sort | uniq | wc -l
#define REGS_CAP 32
#define NAME_MAX 12


typedef enum { GT, LT, GE, LE, EQ, NE } oper;


typedef struct {
    char name[NAME_MAX];
    int val;
} reg;


typedef struct {
    reg *reg;
    oper oper;
    int val;
} cond;


typedef struct {
    reg *target;
    bool inc;
    int amt;
    cond cond;
} instr;


static int regcmp(const void *a, const void *b);
static void strtoinstr(char *s, instr *i, reg *registers, size_t len);
static int max_reg(reg *registers, size_t len);


int
main(void)
{
    size_t regs_len = 0;
    reg regs[REGS_CAP] = {0};

    if(fseek(stdin, 0, SEEK_SET) != 0) {
        fprintf(stderr, "ERR stdin not seekable\n");
        exit(1);
    }

    // Collect register names.
    char line[128] = {0};
    while (fgets(line, 128, stdin)) {
        reg r = {0};
        line[strcspn(line, " ")] = '\0';
        strcpy(r.name, line);

        // Seen this name already?
        bool seen = bsearch(&r, regs, regs_len, sizeof (reg), regcmp);
        if (!seen) {
            regs[regs_len++] = r;
            qsort(regs, regs_len, sizeof (reg), regcmp);
        }
    }

    rewind(stdin);
    int all_time_high = 0;
    while (fgets(line, 128, stdin)) {
        instr i = {0};
        strtoinstr(line, &i, regs, regs_len);

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
        int cur_max = max_reg(regs, regs_len);
        if (cur_max > all_time_high) {all_time_high = cur_max;}
    }

    printf("all time high: %d, max at end: %d\n",
           all_time_high,
           max_reg(regs, regs_len));

    return 0;
}


int
regcmp(const void *a, const void *b)
{
    return strcmp(((const reg *)a)->name, ((const reg *)b)->name);
}


void
strtoinstr(char *s, instr *i, reg *registers, size_t len)
{
    reg r = {0};
    char *tok = strtok(s, " ");
    strcpy(r.name, tok);
    //i->target = darray_bsearch(registers, tok, regcmp);
    i->target = bsearch(&r, registers, len, sizeof (reg), regcmp);

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
    strcpy(r.name, tok);
    i->cond.reg = bsearch(&r, registers, len, sizeof (reg), regcmp);
    //darray_bsearch(registers, tok, regcmp);

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
max_reg(reg *registers, size_t len)
{
    int max = registers[0].val;
    for (size_t i = 1; i < len; i++) {
        int other = registers[i].val;
        if (other > max) {max = other;}
    }
    return max;
}
